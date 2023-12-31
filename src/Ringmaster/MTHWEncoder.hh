#ifndef MTHW_ENCODER_HH
#define MTHW_ENCODER_HH

#include <deque>
#include <map>
#include <memory>
#include <optional>

#include <cuda.h>
#include <iostream>
#include <iomanip>
#include <exception>
#include <stdexcept>
#include <memory>
#include <functional>
#include <stdint.h>

#include "NvDecoder/NvDecoder.h"
#include "NvEncoder/NvEncoderCuda.h"
#include "NvEncoderCLIOptions.h"
#include "NvCodecUtils.h"
#include "FFmpegStreamer.h"
#include "FFmpegDemuxer.h"
#include "ColorSpace.h"

#include "timestamp.hh"
#include "exception.hh"    
#include "image.hh"
#include "protocol.hh"
#include "file_descriptor.hh" 

#include "EncMultiInstance.h"

enum OutputFormat
{
    native = 0, bgra, bgra64
};

class MTHWEncoder
{
public:
  // initialize a VP9 encoder
  MTHWEncoder(const uint16_t nWidth,
          const uint16_t nHeight,
          const uint16_t frame_rate,
          const std::string & output_path = "");
  ~MTHWEncoder();

  // encode raw_img and packetize into datagrams
  void compress_frame(const std::unique_ptr<uint8_t[]>& pHostFrame);

  // add a transmitted but unacked datagram (except retransmissions) to unacked
  void add_unacked(const FrameDatagram & datagram);
  void add_unacked(FrameDatagram && datagram);

  // handle ACK
  void handle_ack(const std::shared_ptr<AckMsg> & ack);

  // output stats every second and reset some of them
  void output_periodic_stats();

  // accessors
  uint32_t frame_id() const { return frame_id_; }
  std::deque<FrameDatagram> & send_buf() { return send_buf_; }
  std::map<SeqNum, FrameDatagram> & unacked() { return unacked_; }

  // mutators
  void set_verbose(const bool verbose) { verbose_ = verbose; }
  void set_target_bitrate(const unsigned int bitrate_kbps);

  //
  uint64_t getEncodedFrameSize() { return vidEncThreads[0].encSession->GetFrameSize(); }

  // forbid copying and moving
  MTHWEncoder(const MTHWEncoder & other) = delete;
  const MTHWEncoder & operator=(const MTHWEncoder & other) = delete;
  MTHWEncoder(MTHWEncoder && other) = delete;
  MTHWEncoder & operator=(MTHWEncoder && other) = delete;

private:
  uint16_t nWidth_;
  uint16_t nHeight_;
  uint16_t frame_rate_;
  std::optional<FileDescriptor> output_fd_;

  /////////////////////////////////////////
  // nv codec
  NvEncoderInitParam EncodeCLIOptions;
  NV_ENC_BUFFER_FORMAT eInputFormat = NV_ENC_BUFFER_FORMAT_IYUV;
  OutputFormat eOutputFormat = native;
  int iGpu = 0;
  bool bBgra64 = false;
  std::exception_ptr encExceptionPtr;
  std::exception_ptr decExceptionPtr;

  CUcontext cuContext {NULL};
  NV_ENC_INITIALIZE_PARAMS initializeParams = { NV_ENC_INITIALIZE_PARAMS_VER }; 
  NV_ENC_RECONFIGURE_PARAMS reconfigureParams = {NV_ENC_RECONFIGURE_PARAMS_VER};
  NV_ENC_PIC_PARAMS picParams = {NV_ENC_PIC_PARAMS_VER};
  NV_ENC_CONFIG encodeConfig = { NV_ENC_CONFIG_VER };
  NV_ENC_CONFIG reInitCodecConfig = { NV_ENC_CONFIG_VER };

  int8_t* qpDeltaMapArray;
  int qpDeltaMapArraySize;
  
  // container
  std::vector<std::vector<uint8_t>> vPacket; 
  /////////////////////////////////////////
  int nThread = 1;
  uint64_t numFramesPerVideoPortion = 1;
  // std::vector<ThreadData> vidEncThreads(nThread);
  // std::vector<IOEncoderMem> ioVideoMem(nThread);
  std::vector<ThreadData> vidEncThreads;
  std::vector<IOEncoderMem> ioVideoMem;
  encodeData outputEncodeData;
  EncodedFrameData outputFrameData;
  

  /////////////////////////////////////////
  
  
  
  FrameType curr_frame_type_ {FrameType::NONKEY};
  
  // print debugging info
  bool verbose_ {false};

  // current target bitrate
  unsigned int target_bitrate_ {0};

  // frame ID to encode
  uint32_t frame_id_ {0};

  // queue of datagrams (packetized video frames) to send
  std::deque<FrameDatagram> send_buf_ {};

  // unacked datagrams
  std::map<SeqNum, FrameDatagram> unacked_ {};

  // RTT-related
  std::optional<unsigned int> min_rtt_us_ {};
  std::optional<double> ewma_rtt_us_ {};
  static constexpr double ALPHA = 0.2;

  // performance stats
  unsigned int num_encoded_frames_ {0};
  double total_encode_time_ms_ {0.0};
  double max_encode_time_ms_ {0.0};

  // constants
  static constexpr unsigned int MAX_NUM_RTX = 3;
  static constexpr uint64_t MAX_UNACKED_US = 1000 * 1000; // 1 second

  // track RTT
  void add_rtt_sample(const unsigned int rtt_us);

  // encode the raw frame stored in 'raw_img'
  void encode_frame(const std::unique_ptr<uint8_t[]>& pHostFrame);

  // packetize the just encoded frame (stored in context_) and return its size
  size_t packetize_encoded_frame(std::vector<std::vector<uint8_t>> &vPacket, uint16_t width, uint16_t height);

  inline void gatherEncodedData(std::vector<uint8_t>& encOutBuf, 
  uint8_t* hostOutVidBuf, uint64_t &totalBitStreamSize, std::vector<EncodedFrameData>& hostEncodedData);

  void asyncEncode(ConcurrentQueue<encodeData>& encodeQueue, std::atomic<bool>& encoderWorking, 
    const std::unique_ptr<uint8_t[]>& pHostFrame);

};

#endif /* ENCODER_HH */