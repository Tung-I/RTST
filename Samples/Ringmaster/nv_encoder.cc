#include <sys/sysinfo.h>
#include <cassert>
#include <iostream>
#include <string>
#include <stdexcept>
#include <chrono>
#include <algorithm>
#include <limits>

#include "nv_encoder.hh"
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

enum OutputFormat
{
    native = 0, bgra, bgra64
};

std::vector<std::string> vstrOutputFormatName = 
{
    "native", "bgra", "bgra64"
};

Encoder::Encoder(const uint16_t default_width,
                 const uint16_t default_height,
                 const uint16_t frame_rate,
                 const string & output_path)
  : default_width_(default_width), default_height_(default_height),
    frame_rate_(frame_rate), output_fd_()
    {
  // open the output file
  if (not output_path.empty()) {
    output_fd_ = FileDescriptor(check_syscall(
        open(output_path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644)));
  }

  // initialize encoder parameters
  std::string CommandLineParam = 
    "-i /home/tungi/datasets/SJTU8K/4k_runner_pano_8s.yuv -o /home/tungi/datasets/SJTU8K/4k_runner_pano_8s.nv12 -s 4096x2048 -if iyuv -of nv12 -codec hevc -gpu 0"
  pEncodeCLIOptions = NvEncoderInitParam(CommandLineParam.c_str(), NULL);
  eInputFormat = NV_ENC_BUFFER_FORMAT_IYUV;
  eOutputFormat = native;

  iGpu = 0;
  bgra64 = false;
  ValidateResolution(default_width, default_height);

  // Check cuda device
  ck(cuInit(0));
  int nGpu = 0;
  ck(cuDeviceGetCount(&nGpu));
  if (iGpu < 0 || iGpu >= nGpu) {
      std::cout << "GPU ordinal out of range. Should be within [" << 0 << ", " << nGpu - 1 << "]" << std::endl;
      return 1;
  }
  CUdevice cuDevice = 0;
  ck(cuDeviceGet(&cuDevice, iGpu));
  char szDeviceName[80];
  ck(cuDeviceGetName(szDeviceName, sizeof(szDeviceName), cuDevice));
  std::cout << "GPU in use: " << szDeviceName << std::endl;

  // Create cuda encoder interface
  ck(cuCtxCreate(cuContext, 0, cuDevice));  
  enc = NvEncoderCuda(cuContext, default_width, default_height, 
    eInputFormat, 3, false, false, false);

  // Set the config and initialize params
  initializeParams.encodeConfig = &encodeConfig; 
  enc.CreateDefaultEncoderParams(&initializeParams, pEncodeCLIOptions->GetEncodeGUID(), 
      pEncodeCLIOptions->GetPresetGUID(), pEncodeCLIOptions->GetTuningInfo());
  encodeConfig.gopLength = NVENC_INFINITE_GOPLENGTH;
  encodeConfig.frameIntervalP = 1;

  if (pEncodeCLIOptions->IsCodecH264())
  {
    encodeConfig.encodeCodecConfig.h264Config.idrPeriod = NVENC_INFINITE_GOPLENGTH;
  }
  else if (pEncodeCLIOptions->IsCodecHEVC())
  {
    encodeConfig.encodeCodecConfig.hevcConfig.idrPeriod = NVENC_INFINITE_GOPLENGTH;
  }
	else
	{
		encodeConfig.encodeCodecConfig.av1Config.idrPeriod = NVENC_INFINITE_GOPLENGTH;
	}
  
  encodeConfig.rcParams.rateControlMode = NV_ENC_PARAMS_RC_CBR;
  encodeConfig.rcParams.multiPass = NV_ENC_MULTI_PASS_DISABLED;
  encodeConfig.rcParams.averageBitRate = (static_cast<unsigned int>(5.0f * initializeParams.encodeWidth * initializeParams.encodeHeight) / (1280 * 720)) * 100000;
  encodeConfig.rcParams.vbvBufferSize = (encodeConfig.rcParams.averageBitRate * initializeParams.frameRateDen / initializeParams.frameRateNum) * 5;  // 5-frame length
  encodeConfig.rcParams.maxBitRate = encodeConfig.rcParams.averageBitRate;
  encodeConfig.rcParams.vbvInitialDelay = encodeConfig.rcParams.vbvBufferSize;

  pEncodeCLIOptions->SetInitParams(&initializeParams, eInputFormat);
  enc.CreateEncoder(&initializeParams);

  // Allocate frame container on host memory
  nHostFrameSize = bBgra64 ? nWidth * nHeight * 8 : enc.GetFrameSize(); 
  pHostFrame = std::unique_ptr<uint8_t[]>(new uint8_t[nHostFrameSize]);

  
}

Encoder::~Encoder()
{
  if (vpx_codec_destroy(&context_) != VPX_CODEC_OK) {
    cerr << "~Encoder(): failed to destroy VPX encoder context" << endl;
  }
}

void Encoder::compress_frame(const std::unique_ptr<uint8_t[]>& pHostFrame)
{

  const auto frame_generation_ts = timestamp_us();

  encode_frame(pHostFrame);

  
  
  // for each packet
            for (std::vector<uint8_t> &packet : vPacket) {
                streamer.Stream(packet.data(), (int)packet.size(), nFrame++);
            }

  // packetize encoder_pkt into datagrams
  const size_t frame_size = packetize_encoded_frame(default_width_, default_height_);

  // output frame information
  if (output_fd_) {
    const auto frame_encoded_ts = timestamp_us();
    const double encode_time_ms = (frame_encoded_ts - frame_generation_ts) / 1000.0;

    output_fd_->write(to_string(frame_id_) + "," +
                      to_string(target_bitrate_) + "," +
                      to_string(frame_size) + "," + 
                      to_string(encode_time_ms) + "," + 
                      double_to_string(*ewma_rtt_us_ / 1000.0) + "\n"); // ms
  }

  // move onto the next frame
  frame_id_++;
}

void Encoder::encode_frame(const std::unique_ptr<uint8_t[]>& pHostFrame)
{
  
  // flush pacakge buffer before encoding a new frame
  if (not unacked_.empty()) {
    const auto & first_unacked = unacked_.cbegin()->second;
    const auto us_since_first_send = timestamp_us() - first_unacked.send_ts;

    // give up if first unacked datagram was initially sent MAX_UNACKED_US ago
    if (us_since_first_send > MAX_UNACKED_US) {
      encode_flags = VPX_EFLAG_FORCE_KF; // force next frame to be key frame
      cerr << "* Recovery: gave up retransmissions and forced a key frame "
           << frame_id_ << endl;

      if (verbose_) {
        cerr << "Giving up on lost datagram: frame_id="
             << first_unacked.frame_id << " frag_id=" << first_unacked.frag_id
             << " rtx=" << first_unacked.num_rtx
             << " us_since_first_send=" << us_since_first_send << endl;
      }

      // clean up
      send_buf_.clear();
      unacked_.clear();
    }
  }

  // copy raw_img into the encoder input buffer
  encoderInputFrame = enc.GetNextInputFrame();  // pointer to the next input buffer
  NvEncoderCuda::CopyToDeviceFrame(cuContext, pHostFrame.get(), 0, (CUdeviceptr)encoderInputFrame->inputPtr,
                (int)encoderInputFrame->pitch,
                enc.GetEncodeWidth(),
                enc.GetEncodeHeight(),
                CU_MEMORYTYPE_HOST,
                encoderInputFrame->bufferFormat,
                encoderInputFrame->chromaOffsets,
                encoderInputFrame->numChromaPlanes);

  // encode it into vPacket
  enc.EncodeFrame(vPacket);


  // encode a frame and calculate encoding time
  const auto encode_start = steady_clock::now();
  check_call(vpx_codec_encode(&context_, raw_img.get_vpx_image(), frame_id_, 1,
                              encode_flags, VPX_DL_REALTIME),
             VPX_CODEC_OK, "failed to encode a frame");
  const auto encode_end = steady_clock::now();
  const double encode_time_ms = duration<double, milli>(
                                encode_end - encode_start).count();

  // track stats in the current period
  num_encoded_frames_++;
  total_encode_time_ms_ += encode_time_ms;
  max_encode_time_ms_ = max(max_encode_time_ms_, encode_time_ms);
}

size_t Encoder::packetize_encoded_frame(uint16_t width, uint16_t height)
{
  const vpx_codec_cx_pkt_t * encoder_pkt;
  vpx_codec_iter_t iter = nullptr;
  unsigned int frames_encoded = 0;
  size_t frame_size = 0;
  
  // get the compressed frame data
  while ((encoder_pkt = vpx_codec_get_cx_data(&context_, &iter))) {
    if (encoder_pkt->kind == VPX_CODEC_CX_FRAME_PKT) {

      // there should be exactly one frame encoded
      frames_encoded++;
      if (frames_encoded > 1) {
        throw runtime_error("Multiple frames were encoded at once");
      }

      // read the returned frame size
      frame_size = encoder_pkt->data.frame.sz;
      assert(frame_size > 0);

      // read the returned frame type
      auto frame_type = FrameType::NONKEY;
      if (encoder_pkt->data.frame.flags & VPX_FRAME_IS_KEY) {
        frame_type = FrameType::KEY;
        if (verbose_) {
          cerr << "Encoded a key frame: frame_id=" << frame_id_ << endl;
        }
      }

      // calculate the total fragments to send
      const uint16_t frag_cnt = narrow_cast<uint16_t>(
          frame_size / (FrameDatagram::max_payload + 1) + 1);

      uint8_t * buf_ptr = static_cast<uint8_t *>(encoder_pkt->data.frame.buf);
      const uint8_t * const buf_end = buf_ptr + frame_size;
      for (uint16_t frag_id = 0; frag_id < frag_cnt; frag_id++) {
        // calculate payload size of the current fragment
        const size_t payload_size = (frag_id < frag_cnt - 1) ?
            FrameDatagram::max_payload : buf_end - buf_ptr;
        // enqueue a datagram
        send_buf_.emplace_back(frame_id_, frame_type, frag_id, frag_cnt, width, height,
          string_view {reinterpret_cast<const char *>(buf_ptr), payload_size});

        buf_ptr += payload_size;
      }
    }
  }

  return frame_size;
}

void Encoder::add_unacked(const FrameDatagram & datagram)
{
  const auto seq_num = make_pair(datagram.frame_id, datagram.frag_id);
  auto [it, success] = unacked_.emplace(seq_num, datagram);

  if (not success) {
    throw runtime_error("datagram already exists in unacked");
  }

  it->second.last_send_ts = it->second.send_ts;
}

void Encoder::add_unacked(FrameDatagram && datagram)
{
  const auto seq_num = make_pair(datagram.frame_id, datagram.frag_id);
  auto [it, success] = unacked_.emplace(seq_num, move(datagram));

  if (not success) {
    throw runtime_error("datagram already exists in unacked");
  }

  it->second.last_send_ts = it->second.send_ts;
}

void Encoder::handle_ack(const shared_ptr<AckMsg> & ack)
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

void Encoder::add_rtt_sample(const unsigned int rtt_us)
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

void Encoder::output_periodic_stats()
{
  cerr << "Frames encoded in the last ~1s: " << num_encoded_frames_ << endl;

  if (num_encoded_frames_ > 0) {
    cerr << "  - Avg/Max encoding time (ms): "
         << double_to_string(total_encode_time_ms_ / num_encoded_frames_)
         << "/" << double_to_string(max_encode_time_ms_) << endl;
  }

  if (min_rtt_us_ and ewma_rtt_us_) {
    cerr << "  - Min/EWMA RTT (ms): " << double_to_string(*min_rtt_us_ / 1000.0)
         << "/" << double_to_string(*ewma_rtt_us_ / 1000.0) << endl;
  }

  // reset all but RTT-related stats
  num_encoded_frames_ = 0;
  total_encode_time_ms_ = 0.0;
  max_encode_time_ms_ = 0.0;
}

void Encoder::set_target_bitrate(const unsigned int bitrate_kbps)
{
  target_bitrate_ = bitrate_kbps;

  cfg_.rc_target_bitrate = target_bitrate_;
  check_call(vpx_codec_enc_config_set(&context_, &cfg_),
             VPX_CODEC_OK, "set_target_bitrate");
}
