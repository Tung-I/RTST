#include <sys/sysinfo.h>
#include <cassert>
#include <iostream>
#include <string>
#include <stdexcept>
#include <chrono>
#include <algorithm>
#include <limits>

#include "HWEncoder.hh"
#include "conversion.hh"
#include "timestamp.hh"

#include <cuda.h>
#include <iomanip>
#include <exception>
#include <memory>
#include <functional>
#include <stdint.h>
#include "NvDecoder/NvDecoder.h"
#include "NvEncoder/NvEncoderCuda.h"
#include "../Utils/NvEncoderCLIOptions.h"
#include "../Utils/NvCodecUtils.h"
#include "../Utils/FFmpegStreamer.h"
#include "../Utils/FFmpegDemuxer.h"
#include "../Utils/ColorSpace.h"

simplelogger::Logger *logger = simplelogger::LoggerFactory::CreateConsoleLogger();

HWEncoder::HWEncoder(const uint16_t nWidth,
                 const uint16_t nHeight,
                 const uint16_t frame_rate,
                 const string & output_path)
  : nWidth_(nWidth), nHeight_(nHeight),
    frame_rate_(frame_rate), output_fd_()
    {
  
  if (not output_path.empty()) {  // for logging
    output_fd_ = FileDescriptor(check_syscall(
        open(output_path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644)));
  }

  ValidateResolution(nWidth, nHeight);

  // string used_codec = "h264";
  string used_codec = "hevc";
  std::string CommandLineParam = "-codec " + used_codec + " -fps " + std::to_string(frame_rate_);
  EncodeCLIOptions = NvEncoderInitParam(CommandLineParam.c_str(), NULL);
  NvEncoderInitParam *pEncodeCLIOptions = &EncodeCLIOptions; 

  // Check cuda device
  ck(cuInit(0));
  int nGpu = 0;
  ck(cuDeviceGetCount(&nGpu));
  if (iGpu < 0 || iGpu >= nGpu) {
      LOG(LogLevel::ERROR) << "GPU ordinal out of range. Should be within [" << 0 << ", " << nGpu - 1 << "]";
      exit(1);

  }
  CUdevice cuDevice = 0;
  ck(cuDeviceGet(&cuDevice, iGpu));
  char szDeviceName[80];
  ck(cuDeviceGetName(szDeviceName, sizeof(szDeviceName), cuDevice));
  LOG(LogLevel::INFO) << "GPU in use: " << szDeviceName;

  // Create nvenc cuda interface
  ck(cuCtxCreate(&cuContext, 0, cuDevice));  
  penc = new NvEncoderCuda(cuContext, nWidth, nHeight, eInputFormat, 3, false, false, false);
   
  // Set the config and initialize params
  initializeParams.encodeConfig = &encodeConfig; 
  penc->CreateDefaultEncoderParams(&initializeParams, pEncodeCLIOptions->GetEncodeGUID(), 
      pEncodeCLIOptions->GetPresetGUID(), pEncodeCLIOptions->GetTuningInfo());

  encodeConfig.gopLength = NVENC_INFINITE_GOPLENGTH;
  // encodeConfig.gopLength = 30;
  encodeConfig.rcParams.disableIadapt = 1;  // disable adaptive insertion of intra frames and B frames
  encodeConfig.rcParams.disableBadapt = 1;
  initializeParams.enablePTD == 1;  // the decision of determining the picture type will be taken by NVENCODE API


  if (pEncodeCLIOptions->IsCodecH264()){
    encodeConfig.encodeCodecConfig.h264Config.h264VUIParameters.videoSignalTypePresentFlag = 1;
    encodeConfig.encodeCodecConfig.h264Config.h264VUIParameters.colourDescriptionPresentFlag = 1;
    encodeConfig.encodeCodecConfig.h264Config.h264VUIParameters.colourMatrix = NV_ENC_VUI_MATRIX_COEFFS_FCC;
  }else{
    encodeConfig.encodeCodecConfig.hevcConfig.hevcVUIParameters.videoSignalTypePresentFlag = 1;
    encodeConfig.encodeCodecConfig.hevcConfig.hevcVUIParameters.colourDescriptionPresentFlag = 1;
    encodeConfig.encodeCodecConfig.hevcConfig.hevcVUIParameters.colourMatrix = NV_ENC_VUI_MATRIX_COEFFS_FCC;
  }
  if (pEncodeCLIOptions->IsCodecH264()){
    encodeConfig.encodeCodecConfig.h264Config.idrPeriod = NVENC_INFINITE_GOPLENGTH;
  }else if (pEncodeCLIOptions->IsCodecHEVC()){
    encodeConfig.encodeCodecConfig.hevcConfig.idrPeriod = NVENC_INFINITE_GOPLENGTH;
  }else{
		encodeConfig.encodeCodecConfig.av1Config.idrPeriod = NVENC_INFINITE_GOPLENGTH;
	}
  
  encodeConfig.rcParams.rateControlMode = NV_ENC_PARAMS_RC_CBR;
  encodeConfig.rcParams.multiPass = NV_ENC_MULTI_PASS_DISABLED;
  encodeConfig.rcParams.averageBitRate = 
    (static_cast<unsigned int>(5.0f * initializeParams.encodeWidth * initializeParams.encodeHeight) / (1280 * 720)) * 100000;
  encodeConfig.rcParams.vbvBufferSize =  // 5-frame length
    (encodeConfig.rcParams.averageBitRate * initializeParams.frameRateDen / initializeParams.frameRateNum) * 5; 
  encodeConfig.rcParams.maxBitRate = encodeConfig.rcParams.averageBitRate;
  encodeConfig.rcParams.vbvInitialDelay = encodeConfig.rcParams.vbvBufferSize;

  if (!penc->GetCapabilityValue(pEncodeCLIOptions->GetEncodeGUID(), NV_ENC_CAPS_SUPPORT_EMPHASIS_LEVEL_MAP)){
    LOG(LogLevel::INFO) << "Encoder does not support emphasis level map; use delta map instead";
    
    encodeConfig.rcParams.qpMapMode = NV_ENC_QP_MAP_DISABLED;
    qpDeltaMapArraySize = 0;
    qpDeltaMapArray = NULL;
    
    //////////////////////////////////////////////////////////////////////// Delta QP (unsupported for now)
    // encodeConfig.rcParams.qpMapMode = NV_ENC_QP_MAP_DELTA;
    // int widthInMacroblocks = nWidth_ / 32;
    // int heightInMacroblocks = nHeight_ / 32;
    // qpDeltaMapArraySize = widthInMacroblocks * heightInMacroblocks;

    // qpDeltaMapArray = (int8_t*)malloc(qpDeltaMapArraySize * sizeof(int8_t));
    // memset(qpDeltaMapArray, 0, qpDeltaMapArraySize);

    // for (int y = 0; y < heightInMacroblocks; y++) {
    //   for (int x = 0; x < widthInMacroblocks; x++) {
    //       int index = y * widthInMacroblocks + x;
    //       // if (x > widthInMacroblocks / 2 && y > heightInMacroblocks / 2){
    //       if (x < widthInMacroblocks / 2 ){
    //           qpDeltaMapArray[index] = NV_ENC_EMPHASIS_MAP_LEVEL_5; 
    //       } else {
    //           qpDeltaMapArray[index] = NV_ENC_EMPHASIS_MAP_LEVEL_0;
    //       }
    //   }
    // }
    ////////////////////////////////////////////////////////////////////////
  }
  else{
    LOG(LogLevel::INFO) << "Enable emphasis level map";
    encodeConfig.rcParams.qpMapMode = NV_ENC_QP_MAP_EMPHASIS;

    int widthInMacroblocks = nWidth_ / 16;
    int heightInMacroblocks = nHeight_ / 16;
    qpDeltaMapArraySize = widthInMacroblocks * heightInMacroblocks;

    qpDeltaMapArray = (int8_t*)malloc(qpDeltaMapArraySize * sizeof(int8_t));
    memset(qpDeltaMapArray, 0, qpDeltaMapArraySize);

    for (int i = 0; i < qpDeltaMapArraySize; i++) {
        if (i < qpDeltaMapArraySize / 2) {
            qpDeltaMapArray[i] = NV_ENC_EMPHASIS_MAP_LEVEL_0; 
        } else {
            qpDeltaMapArray[i] = NV_ENC_EMPHASIS_MAP_LEVEL_5;
        }
    }
  }
  
  // Create the encoder interface
  pEncodeCLIOptions->SetInitParams(&initializeParams, eInputFormat);
  penc->CreateEncoder(&initializeParams);
  picParams.encodePicFlags = 0;
}

HWEncoder::~HWEncoder()
{
  penc->DestroyEncoder();
}

void HWEncoder::compress_frame(const std::unique_ptr<uint8_t[]>& pHostFrame)
{
  const auto frame_generation_ts = timestamp_us();
  curr_frame_type_ = FrameType::NONKEY;
  encode_frame(pHostFrame);
  const size_t frame_size = packetize_encoded_frame(vPacket, nWidth_, nHeight_);

  if (output_fd_) {
    const auto frame_encoded_ts = timestamp_us();
    const double encode_time_ms = (frame_encoded_ts - frame_generation_ts) / 1000.0;
    output_fd_->write(to_string(frame_id_) + "," +
                      to_string(target_bitrate_) + "," +
                      to_string(frame_size) + "," + 
                      to_string(encode_time_ms) + "," + 
                      double_to_string(*ewma_rtt_us_ / 1000.0) + "\n"); // ms
  }
}

void HWEncoder::encode_frame(const std::unique_ptr<uint8_t[]>& pHostFrame)
{

  picParams.encodePicFlags = 0;
  curr_frame_type_ = FrameType::UNKNOWN;

  // Clean up if we've given up on retransmissions
  if (not unacked_.empty()) {
    const auto & first_unacked = unacked_.cbegin()->second;
    const auto us_since_first_send = timestamp_us() - first_unacked.send_ts;

    if (us_since_first_send > MAX_UNACKED_US) {
      picParams.encodePicFlags = NV_ENC_PIC_FLAG_FORCEINTRA;  // force an I frame
      curr_frame_type_ = FrameType::KEY;

      LOG(LogLevel::WARNING) << endl << "* Recovery: gave up retransmissions and forced a key frame "
           << frame_id_ << endl;
      if (verbose_) {
        LOG(LogLevel::WARNING) << endl << "Giving up on lost datagram: frame_id="
             << first_unacked.frame_id << " frag_id=" << first_unacked.frag_id
             << " rtx=" << first_unacked.num_rtx
             << " us_since_first_send=" << us_since_first_send << endl;
      }

      // clean up
      send_buf_.clear();
      unacked_.clear();
    }
  }

  // copy host frame into the encoder input buffer
  const NvEncInputFrame* encoderInputFrame = penc->GetNextInputFrame();  // pointer to the next input buffer

  NvEncoderCuda::CopyToDeviceFrame(cuContext,
      pHostFrame.get(),
      0,
      (CUdeviceptr)encoderInputFrame->inputPtr,
      (int)encoderInputFrame->pitch,
      penc->GetEncodeWidth(),
      penc->GetEncodeHeight(),
      CU_MEMORYTYPE_HOST,
      encoderInputFrame->bufferFormat,
      encoderInputFrame->chromaOffsets,
      encoderInputFrame->numChromaPlanes
  );

  // ////////////////////////////////////////////////////// QP Delta (unsupported for now)
  // picParams.qpDeltaMapSize = qpDeltaMapArraySize;
  // picParams.qpDeltaMap = qpDeltaMapArray;
  /////////////////////////////////////////////////////////

  // encode it into vPacket
  const auto encode_start = std::chrono::steady_clock::now();
  penc->EncodeFrame(vPacket, &picParams);
  const auto encode_end = std::chrono::steady_clock::now();
  const double encode_time_ms = std::chrono::duration<double, milli>(encode_end - encode_start).count();

  // track stats in the current period
  num_encoded_frames_++;
  total_encode_time_ms_ += encode_time_ms;
  max_encode_time_ms_ = max(max_encode_time_ms_, encode_time_ms);
}

size_t HWEncoder::packetize_encoded_frame(std::vector<std::vector<uint8_t>> &vPacket, uint16_t width, uint16_t height)
{
  if (vPacket.empty()) {
    return 0;
  }

  size_t frame_size = 0;  //bytes
  
  auto frame_type  = curr_frame_type_;
  if (frame_type == FrameType::KEY) {
    if (verbose_) {
      cerr << "Encoded a key frame: frame_id=" << frame_id_ << endl;
    }
  }

  // Calculate the number of fragments
  uint16_t frag_cnt = 0;
  for (const auto &packet : vPacket) {
    if (packet.empty()) {
      continue;
    }
    frag_cnt += (packet.size() + FrameDatagram::max_payload - 1) / FrameDatagram::max_payload;      
  }
  
  // packetize the encoded frame
  uint16_t frag_id = 0;
  for (const auto &packet : vPacket) {
    size_t packet_size = packet.size();
    size_t processed = 0;  // Amount processed from the current packet

    while (processed < packet_size) {
      size_t payload_size = std::min(FrameDatagram::max_payload, packet_size - processed);
      const uint8_t* start_ptr = packet.data() + processed;
      std::string_view payload(reinterpret_cast<const char*>(start_ptr), payload_size);
      frame_size += payload_size;
 
      send_buf_.emplace_back(frame_id_, frame_type, frag_id, frag_cnt, width, height, payload);
      frag_id++;

      processed += payload_size;
    }
  }

  frame_id_++;
  return frame_size;
}

void HWEncoder::add_unacked(const FrameDatagram & datagram)
{
  const auto seq_num = make_pair(datagram.frame_id, datagram.frag_id);
  auto [it, success] = unacked_.emplace(seq_num, datagram);

  if (not success) {
    throw runtime_error("datagram already exists in unacked");
  }

  it->second.last_send_ts = it->second.send_ts;
}

void HWEncoder::add_unacked(FrameDatagram && datagram)
{
  const auto seq_num = make_pair(datagram.frame_id, datagram.frag_id);
  auto [it, success] = unacked_.emplace(seq_num, move(datagram));

  if (not success) {
    throw runtime_error("datagram already exists in unacked");
  }

  it->second.last_send_ts = it->second.send_ts;
}

void HWEncoder::handle_ack(const shared_ptr<AckMsg> & ack)
{
  const auto curr_ts = timestamp_us();

  // observed an RTT sample
  add_rtt_sample(curr_ts - ack->send_ts);

  // find the acked datagram in 'unacked_'
  const auto acked_seq_num = make_pair(ack->frame_id, ack->frag_id);
  auto acked_it = unacked_.find(acked_seq_num);

  if (acked_it == unacked_.end()) {
    // do nothing else if ACK is not for an unacked datagram
    return;
  }

  // retransmit all unacked datagrams before the acked one (backward)
  for (auto rit = make_reverse_iterator(acked_it);
       rit != unacked_.rend(); rit++) {
    auto & datagram = rit->second;

    // skip if a datagram has been retransmitted MAX_NUM_RTX times
    if (datagram.num_rtx >= MAX_NUM_RTX) {
      continue;
    }

    // retransmit if it's the first RTX or the last RTX was about one RTT ago
    if (datagram.num_rtx == 0 or
        curr_ts - datagram.last_send_ts > ewma_rtt_us_.value()) {
      datagram.num_rtx++;
      datagram.last_send_ts = curr_ts;

      // retransmissions are more urgent
      send_buf_.emplace_front(datagram);
    }
  }

  // finally, erase the acked datagram from 'unacked_'
  unacked_.erase(acked_it);
}

void HWEncoder::add_rtt_sample(const unsigned int rtt_us)
{
  // min RTT
  if (not min_rtt_us_ or rtt_us < *min_rtt_us_) {
    min_rtt_us_ = rtt_us;
  }

  // EWMA RTT
  if (not ewma_rtt_us_) {
    ewma_rtt_us_ = rtt_us;
  } else {
    ewma_rtt_us_ = ALPHA * rtt_us + (1 - ALPHA) * (*ewma_rtt_us_);
  }
}

void HWEncoder::output_periodic_stats()
{
  LOG(LogLevel::INFO) << "Frames encoded in the last ~1s: " << num_encoded_frames_;

  if (num_encoded_frames_ > 0) {
    LOG(LogLevel::INFO) << "  - Avg/Max encoding time (ms): "
         << double_to_string(total_encode_time_ms_ / num_encoded_frames_)
         << "/" << double_to_string(max_encode_time_ms_);
  }

  if (min_rtt_us_ and ewma_rtt_us_) {

    LOG(LogLevel::INFO) << "  - Min/EWMA RTT (ms): " << double_to_string(*min_rtt_us_ / 1000.0) 
        << "/" << double_to_string(*ewma_rtt_us_ / 1000.0);
  }

  // reset all but RTT-related stats
  num_encoded_frames_ = 0;
  total_encode_time_ms_ = 0.0;
  max_encode_time_ms_ = 0.0;
}

void HWEncoder::set_target_bitrate(const unsigned int bitrate_kbps)
{
  target_bitrate_ = bitrate_kbps * 1000;  // bps

  // Update reconfigure params
  memcpy(&reconfigureParams.reInitEncodeParams, &initializeParams, sizeof(initializeParams));
  memcpy(&reInitCodecConfig, initializeParams.encodeConfig, sizeof(reInitCodecConfig));
  reconfigureParams.reInitEncodeParams.encodeConfig = &reInitCodecConfig;
  reconfigureParams.reInitEncodeParams.encodeConfig->rcParams.averageBitRate = target_bitrate_;
  reconfigureParams.reInitEncodeParams.encodeConfig->rcParams.vbvBufferSize = 
          reconfigureParams.reInitEncodeParams.encodeConfig->rcParams.averageBitRate * 
          reconfigureParams.reInitEncodeParams.frameRateDen / reconfigureParams.reInitEncodeParams.frameRateNum;
  reconfigureParams.reInitEncodeParams.encodeConfig->rcParams.maxBitRate = 
          reconfigureParams.reInitEncodeParams.encodeConfig->rcParams.averageBitRate;
  reconfigureParams.reInitEncodeParams.encodeConfig->rcParams.vbvInitialDelay = 
          reconfigureParams.reInitEncodeParams.encodeConfig->rcParams.vbvBufferSize;
  
  penc->Reconfigure(&reconfigureParams);
}
