#include <sys/sysinfo.h>
#include <cassert>
#include <iostream>
#include <string>
#include <stdexcept>
#include <chrono>
#include <algorithm>
#include <limits>

#include "MTHWEncoder.hh"
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

MTHWEncoder::MTHWEncoder(const uint16_t nWidth,
                 const uint16_t nHeight,
                 const uint16_t frame_rate,
                 const string & output_path)
  : nWidth_(nWidth), nHeight_(nHeight), frame_rate_(frame_rate), output_fd_(),
  vidEncThreads(nThread), ioVideoMem(nThread)
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
  // Set the config and initialize params
  initializeParams.encodeConfig = &encodeConfig; 

  // Create and initialize array of data required for each encoding session thread 
  ck(cuCtxCreate(&(cuContext), CU_CTX_SCHED_BLOCKING_SYNC, cuDevice)); // Create single CUDA context
  for (int i = 0; i < nThread; i++) {
    vidEncThreads[i].cuContext = &cuContext; // same CUDA context for every encoding session thread
    // safely cast nWidth to uint32_t

    vidEncThreads[i].encSession = make_it_unique<NvEncoderCuda>(cuContext, 
      static_cast<int32_t>(nWidth), static_cast<int32_t>(nHeight), eInputFormat);
    vidEncThreads[i].encSession->CreateDefaultEncoderParams(&initializeParams, 
      pEncodeCLIOptions->GetEncodeGUID(), pEncodeCLIOptions->GetPresetGUID(), pEncodeCLIOptions->GetTuningInfo());
    
    
    initializeParams.enablePTD == 1;  // to be debugged
    encodeConfig.gopLength = NVENC_INFINITE_GOPLENGTH;
    encodeConfig.rcParams.disableIadapt = 1;  
    encodeConfig.rcParams.disableBadapt = 1;

    // encodeConfig.encodeCodecConfig.hevcConfig.enableIntraRefresh = 1;
    // encodeConfig.encodeCodecConfig.hevcConfig.intraRefreshCnt = 5;
    // encodeConfig.encodeCodecConfig.hevcConfig.intraRefreshPeriod = 30;
    encodeConfig.encodeCodecConfig.hevcConfig.hevcVUIParameters.videoSignalTypePresentFlag = 1;
    encodeConfig.encodeCodecConfig.hevcConfig.hevcVUIParameters.colourDescriptionPresentFlag = 1;
    encodeConfig.encodeCodecConfig.hevcConfig.hevcVUIParameters.colourMatrix = NV_ENC_VUI_MATRIX_COEFFS_FCC;
    encodeConfig.encodeCodecConfig.hevcConfig.idrPeriod = NVENC_INFINITE_GOPLENGTH;
    encodeConfig.rcParams.rateControlMode = NV_ENC_PARAMS_RC_CBR;
    encodeConfig.rcParams.multiPass = NV_ENC_MULTI_PASS_DISABLED;
    

    pEncodeCLIOptions->SetInitParams(&initializeParams, eInputFormat);
    vidEncThreads[i].encSession->CreateEncoder(&initializeParams);
    vidEncThreads[i].cuStream.reset(new NvCUStream(cuContext, 1, vidEncThreads[i].encSession)); // each encoding session thread is going to use one cuda stream
  }

  // Allocate all the required memory for IO 
  uint64_t frameSize = vidEncThreads[0].encSession->GetFrameSize();
  uint64_t sizePerVideoPortion = numFramesPerVideoPortion * frameSize;
  
  for (int i = 0; i < nThread; i++) {
    ioVideoMem[i].hostInBuf.readyToEdit = true;
    ck(cuMemAllocHost((void**)&ioVideoMem[i].hostInBuf.data, frameSize)); // Allocate pinned memory for input RAW frame
    ioVideoMem[i].hostOutBuf.readyToEdit = true;
    ck(cuMemAllocHost((void**)&ioVideoMem[i].hostOutBuf.data, sizePerVideoPortion)); // Allocate pinned memory for output compressed video portions
  }
  
}

MTHWEncoder::~MTHWEncoder()
{
  for (int i = 0; i < nThread; i++) vidEncThreads[i].encSession->DestroyEncoder();
}

void MTHWEncoder::compress_frame(const std::unique_ptr<uint8_t[]>& pHostFrame)
{
  const auto frame_generation_ts = timestamp_us();

  
  curr_frame_type_ = FrameType::NONKEY;
  encode_frame(pHostFrame);

  const size_t frame_size = packetize_encoded_frame(
    vPacket, nWidth_, nHeight_);

  // Write logging info
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

void MTHWEncoder::encode_frame(const std::unique_ptr<uint8_t[]>& pHostFrame)
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

  const auto encode_start = std::chrono::steady_clock::now();

  /////////////////////////////////////////////////////////////////////
  // Create encode work queues
  std::vector<ConcurrentQueue<encodeData>> encodeQueue(nThread);
  for (int i = 0; i < nThread; i++){
    // video ENCODING thread work queue generation
    encodeData currEncData;
    currEncData.offset = 0;
    currEncData.numFrames = 1;
    currEncData.threadData = &vidEncThreads[i];
    currEncData.vidPortionNum = 0;
    currEncData.vidThreadIdx = i;
    currEncData.ioVideoMem = &ioVideoMem[i];
    currEncData.isLast = 1; // check if last to end thread
    currEncData.isSingleThread = (nThread == 1);
    encodeQueue[i].push_back(currEncData); // queue encode work
  }

  // Launch encoding threads 
  try
	{
    std::atomic<bool> encoderWorking(true);
    std::vector<std::thread> encodeThread(nThread);
    for (int i = 0; i < nThread; i++){
      LOG(LogLevel::INFO) << "Spawn thread " << i;
      encodeThread[i] = std::thread(&MTHWEncoder::asyncEncode, this, 
        std::ref(encodeQueue[i]), std::ref(encoderWorking), std::ref(pHostFrame));
    }
    StopWatch processingTime;
    processingTime.Start();
    for (int i = 0; i < nThread; i++) encodeThread[i].join();
    double pT = processingTime.Stop();
  }
  catch (const std::exception &ex)
	{
    LOG(LogLevel::ERROR) << "Error: " << ex.what();
	}

  // TODO: make a decision on which thread's output to acquire
  //debug
  LOG(LogLevel::INFO) << "encodeQueue[0] size: " << encodeQueue[0].size();
  outputEncodeData = encodeQueue[0].pop_front();
  LOG(LogLevel::INFO) << "After pop";
  std::cout << outputEncodeData.ioVideoMem->hostEncodedData.size() << endl;
  // raise an exception if the output is empty

  // std::memcpy(vPacket[0].data(), outputEncodeData.ioVideoMem->hostOutBuf.data, 
  //   outputEncodeData.ioVideoMem->hostOutBuf.size);
  /////////////////////////////////////////////////////////////////////

  const auto encode_end = std::chrono::steady_clock::now();
  const double encode_time_ms = std::chrono::duration<double, milli>(encode_end - encode_start).count();

  // track stats in the current period
  num_encoded_frames_++;
  total_encode_time_ms_ += encode_time_ms;
  max_encode_time_ms_ = max(max_encode_time_ms_, encode_time_ms);
}

void MTHWEncoder::asyncEncode(ConcurrentQueue<encodeData>& encodeQueue, std::atomic<bool>& encoderWorking, 
  const std::unique_ptr<uint8_t[]>& pHostFrame)
{
  encodeData enc;
	while (encoderWorking) {
    enc = encodeQueue.pop_front();
    safeBuffer* inSafeBuf = &enc.ioVideoMem->hostInBuf;
		safeBuffer* outSafeBuf = &enc.ioVideoMem->hostOutBuf;

    // std::unique_lock<std::mutex> outLock{ outSafeBuf->mutex }; 
    // while (!outSafeBuf->readyToEdit) {
    //   outSafeBuf->condVarReady.wait(outLock); // wait until OUTPUT buffer is ready to be EDITED
    // }

    enc.ioVideoMem->hostEncodedData.clear(); // clear last ouput data
		uint64_t nFrameSize = enc.threadData->encSession->GetFrameSize();
    uint64_t totalBitStreamSize = 0; // need to keep track of the size of each compressed frame
		ck(cuCtxSetCurrent((CUcontext)enc.threadData->encSession->GetDevice()));
    std::vector<std::vector<uint8_t>> encOutBuf;
    NV_ENC_PIC_PARAMS nvEncPicParams = { NV_ENC_PIC_PARAMS_VER };

    if (frame_id_ == 0) nvEncPicParams.encodePicFlags = NV_ENC_PIC_FLAG_FORCEIDR; // force IDR frame

    for (uint32_t i = 0; i < enc.numFrames; i++){
      ck(cuStreamSynchronize(enc.threadData->cuStream->GetInputCUStream())); // make sure the last memcpy is complete
      const NvEncInputFrame* encoderInputFrame = enc.threadData->encSession->GetNextInputFrame();

      // copy the data in pHostFrame, which is a std::unique_ptr<uint8_t[]>, into inSafeBuf->data
      std::memcpy(inSafeBuf->data, pHostFrame.get(), nFrameSize); 
      // do async frame copy from host to device
      NvEncoderCuda::CopyToDeviceFrame((CUcontext)enc.threadData->encSession->GetDevice(),
        (uint8_t*)inSafeBuf->data,
        0,
        (CUdeviceptr)encoderInputFrame->inputPtr,
        encoderInputFrame->pitch,
        enc.threadData->encSession->GetEncodeWidth(),
        enc.threadData->encSession->GetEncodeHeight(),
        CU_MEMORYTYPE_HOST,
        encoderInputFrame->bufferFormat,
        encoderInputFrame->chromaOffsets,
        encoderInputFrame->numChromaPlanes,
        false,
        enc.threadData->cuStream->GetInputCUStream()
      ); 

      enc.threadData->encSession->EncodeFrame(encOutBuf, &nvEncPicParams);

      for (uint32_t j = 0; j < encOutBuf.size(); ++j) { // gather encoded data in output buffer
        gatherEncodedData(encOutBuf[j], outSafeBuf->data, totalBitStreamSize, enc.ioVideoMem->hostEncodedData);
      }
    }
    if (!enc.isSingleThread || enc.isLast) {
      enc.threadData->encSession->EndEncode(encOutBuf); // get last compressed frames
      for (uint32_t j = 0; j < encOutBuf.size(); ++j) { // gather encoded data in output buffer to write to file later
        gatherEncodedData(encOutBuf[j], outSafeBuf->data, totalBitStreamSize, enc.ioVideoMem->hostEncodedData);
      }
    }

    outSafeBuf->readyToEdit = false; // OUTPUT buffer is ready to be READ
    LOG(LogLevel::INFO) << "OUTPUT buffer is ready to be READ";
		outSafeBuf->condVarReady.notify_all();
    LOG(LogLevel::INFO) << "Notified all";
    if (enc.isLast) { // if last end thread
			encoderWorking = false;
			break;
		}
    
  }

}


size_t MTHWEncoder::packetize_encoded_frame(std::vector<std::vector<uint8_t>> &vPacket, uint16_t width, uint16_t height)
{
  if (vPacket.empty()) return 0;
  size_t frame_size = 0;  //bytes
  
  auto frame_type  = curr_frame_type_;
  if (frame_type == FrameType::KEY) {
    if (verbose_) {
      cerr << "Encoded a key frame: frame_id=" << frame_id_ << endl;
    }
  }

  // calculate the number of fragments
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

void MTHWEncoder::add_unacked(const FrameDatagram & datagram)
{
  const auto seq_num = make_pair(datagram.frame_id, datagram.frag_id);
  auto [it, success] = unacked_.emplace(seq_num, datagram);

  if (not success) {
    throw runtime_error("datagram already exists in unacked");
  }

  it->second.last_send_ts = it->second.send_ts;
}

void MTHWEncoder::add_unacked(FrameDatagram && datagram)
{
  const auto seq_num = make_pair(datagram.frame_id, datagram.frag_id);
  auto [it, success] = unacked_.emplace(seq_num, move(datagram));

  if (not success) {
    throw runtime_error("datagram already exists in unacked");
  }

  it->second.last_send_ts = it->second.send_ts;
}

void MTHWEncoder::handle_ack(const shared_ptr<AckMsg> & ack)
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

void MTHWEncoder::add_rtt_sample(const unsigned int rtt_us)
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

void MTHWEncoder::output_periodic_stats()
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

void MTHWEncoder::set_target_bitrate(const unsigned int bitrate_kbps)
{
  target_bitrate_ = bitrate_kbps * 1000;  // bps

  // update the reconfigure params
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

  // reconfigure all the encoding sessions
  for (int i = 0; i < nThread; i++)
    vidEncThreads[i].encSession->Reconfigure(&reconfigureParams);
}

inline void MTHWEncoder::gatherEncodedData(std::vector<uint8_t>& encOutBuf, 
  uint8_t* hostOutVidBuf, uint64_t &totalBitStreamSize, std::vector<EncodedFrameData>& hostEncodedData)
{
	EncodedFrameData frameData;
	frameData.offset = 0;
	frameData.data = hostOutVidBuf + totalBitStreamSize;
	frameData.size = static_cast<uint32_t>(encOutBuf.size()); // get size of the bitstream chunk
	std::memcpy(frameData.data, reinterpret_cast<char*>(encOutBuf.data()), encOutBuf.size());
	totalBitStreamSize += frameData.size + frameData.offset; // increment copied size
	hostEncodedData.push_back(std::ref(frameData));
}
