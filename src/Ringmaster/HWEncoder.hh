#ifndef HW_ENCODER_HH
#define HW_ENCODER_HH

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

enum OutputFormat
{
  native = 0,
  bgra,
  bgra64
};

class HWEncoder
{
public:
  HWEncoder(const uint16_t nWidth,
              const uint16_t nHeight,
              const uint16_t frame_rate,
              const std::string &output_path = "");
  ~HWEncoder();

  // Cuda encoder interface
  NvEncoderCuda *penc = NULL;

  // Call by sender
  void compress_frame(const std::unique_ptr<uint8_t[]> &pHostFrame);

  // Add a transmitted but unacked datagram to the queue
  void add_unacked(const FrameDatagram &datagram);
  void add_unacked(FrameDatagram &&datagram);

  // Call whenever ACK is received
  void handle_ack(const std::shared_ptr<AckMsg> &ack);

  // Return the size of the encoded frame
  uint64_t getEncodedFrameSize() { return penc->GetFrameSize(); }

  // Print encoding stats on the console
  void output_periodic_stats();

  // Accessors
  uint32_t frame_id() const { return frame_id_; }
  std::deque<FrameDatagram> &send_buf() { return send_buf_; }
  std::map<SeqNum, FrameDatagram> &unacked() { return unacked_; }

  // Mutators
  void set_verbose(const bool verbose) { verbose_ = verbose; }
  void set_target_bitrate(const unsigned int bitrate_kbps);

  // Forbid copying and moving
  HWEncoder(const HWEncoder &other) = delete;
  const HWEncoder &operator=(const HWEncoder &other) = delete;
  HWEncoder(HWEncoder &&other) = delete;
  HWEncoder &operator=(HWEncoder &&other) = delete;


private:
  uint16_t nWidth_;
  uint16_t nHeight_;
  uint16_t frame_rate_;
  std::optional<FileDescriptor> output_fd_;

  // NVIDIA Codec related
  NvEncoderInitParam EncodeCLIOptions;
  NV_ENC_BUFFER_FORMAT eInputFormat = NV_ENC_BUFFER_FORMAT_IYUV;
  OutputFormat eOutputFormat = native;
  int iGpu = 0;
  bool bBgra64 = false;
  std::exception_ptr encExceptionPtr;
  std::exception_ptr decExceptionPtr;

  CUcontext cuContext{NULL};
  NV_ENC_INITIALIZE_PARAMS initializeParams = {NV_ENC_INITIALIZE_PARAMS_VER};
  NV_ENC_RECONFIGURE_PARAMS reconfigureParams = {NV_ENC_RECONFIGURE_PARAMS_VER};
  NV_ENC_PIC_PARAMS picParams = {NV_ENC_PIC_PARAMS_VER};
  NV_ENC_CONFIG encodeConfig = {NV_ENC_CONFIG_VER};
  NV_ENC_CONFIG reInitCodecConfig = {NV_ENC_CONFIG_VER};

  int8_t *qpDeltaMapArray;
  int qpDeltaMapArraySize;

  std::vector<std::vector<uint8_t>> vPacket; // container for encoded packets
  int nFrameSize = 0;
  
  // Variables
  FrameType curr_frame_type_{FrameType::NONKEY};
  bool verbose_{false};
  unsigned int target_bitrate_{0};
  uint32_t frame_id_{0};

  // Queue of datagrams to send
  std::deque<FrameDatagram> send_buf_{};

  // Record outstanding datagrams
  std::map<SeqNum, FrameDatagram> unacked_{};

  // Encoding stats
  std::optional<unsigned int> min_rtt_us_{};
  std::optional<double> ewma_rtt_us_{};
  static constexpr double ALPHA = 0.2;
  unsigned int num_encoded_frames_{0};
  double total_encode_time_ms_{0.0};
  double max_encode_time_ms_{0.0};
  void add_rtt_sample(const unsigned int rtt_us);

  // Parameters
  static constexpr unsigned int MAX_NUM_RTX = 3;
  static constexpr uint64_t MAX_UNACKED_US = 1000 * 1000; // 1 second

  // Internal functions
  void encode_frame(const std::unique_ptr<uint8_t[]> &pHostFrame);
  size_t packetize_encoded_frame(std::vector<std::vector<uint8_t>> &vPacket, uint16_t width, uint16_t height);
};

#endif /* ENCODER_HH */