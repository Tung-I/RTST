#ifndef HW_DECODER
#define HW_DECODER

#include <map>
#include <vector>
#include <deque>
#include <optional>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <thread>

#include "protocol.hh"
#include "sdl.hh"
#include "file_descriptor.hh"

#include "NvDecoder/NvDecoder.h"
#include "NvEncoder/NvEncoderCuda.h"
#include "NvEncoderCLIOptions.h"
#include "NvCodecUtils.h"
#include "FFmpegStreamer.h"
#include "FFmpegDemuxer.h"
#include "ColorSpace.h"

enum OutputFormat
{
    native = 0, bgra, bgra64
};

class Frame
{
public:
  Frame(const uint32_t frame_id,
        const FrameType frame_type,
        const uint16_t frag_cnt);

  // Collect fragments of a frame
  bool has_frag(const uint16_t frag_id) const;
  FrameDatagram & get_frag(const uint16_t frag_id);
  const FrameDatagram & get_frag(const uint16_t frag_id) const;
  void insert_frag(const FrameDatagram & datagram);
  void insert_frag(FrameDatagram && datagram);
  bool complete() const { return null_frags_ == 0; } // if the frame has received all fragments
  std::optional<size_t> frame_size() const;

  // Accessors
  uint32_t id() const { return id_; }
  FrameType type() const { return type_; }
  std::vector<std::optional<FrameDatagram>> & frags() { return frags_; }
  const std::vector<std::optional<FrameDatagram>> & frags() const { return frags_; }
  unsigned int null_frags() const { return null_frags_; }

private:
  uint32_t id_;    // frame ID
  FrameType type_; // frame type

  std::vector<std::optional<FrameDatagram>> frags_; // fragments of this frame
  unsigned int null_frags_; // number of uninitialized fragments
  size_t frame_size_ {0}; // frame size so far

  // Validate if a datagram belongs to this frame
  void validate_datagram(const FrameDatagram & datagram) const;
};

class HWDecoder
{
public:
  enum LazyLevel : int {
    DECODE_DISPLAY = 0,   // decode and display
    DECODE_ONLY = 1,      // decode only but not display
    NO_DECODE_DISPLAY = 2 // neither decode nor display
  };

  HWDecoder(const uint16_t display_width,
          const uint16_t display_height,
          const int lazy_level = 0,
          const std::string & output_path = "");

  void add_datagram(const FrameDatagram & datagram);
  void add_datagram(FrameDatagram && datagram);
  bool next_frame_complete();
  void consume_next_frame();

  void output_periodic_stats();

  // Accessors
  uint32_t next_frame() const { return next_frame_; }

  // Mutators
  void set_verbose(const bool verbose) { verbose_ = verbose; }

  // Forbid copying and moving
  HWDecoder(const HWDecoder & other) = delete;
  const HWDecoder & operator=(const HWDecoder & other) = delete;
  HWDecoder(HWDecoder && other) = delete;
  HWDecoder & operator=(HWDecoder && other) = delete;

private:
  uint16_t display_width_;
  uint16_t display_height_;
  LazyLevel lazy_level_;
  std::optional<FileDescriptor> output_fd_; // only one thread should output
  std::chrono::time_point<std::chrono::steady_clock> decoder_epoch_;

  bool verbose_ {false};

  uint32_t next_frame_ {0};  // next frame ID to decode
  std::map<uint32_t, Frame> frame_buf_ {};

  // Decoding stats
  unsigned int num_decodable_frames_ {0};
  size_t total_decodable_frame_size_ {0}; // bytes
  std::chrono::time_point<std::chrono::steady_clock> last_stats_time_ {};

  // Shared between main (Decoder) and wonFrame_decoded rker threads
  std::mutex mtx_ {};
  std::condition_variable cv_ {};
  std::deque<Frame> shared_queue_ {};

  // Worker thread for decoding and displaying frames
  std::thread worker_ {};

  // common code between the two versions of add_datagram()
  bool add_datagram_common(const FrameDatagram & datagram);

  // advance next frame ID by 'n'
  void advance_next_frame(const unsigned int n = 1);

  // clean up states (such as frame_buf_) up to frame 'frontier'
  void clean_up_to(const uint32_t frontier);

  // worker thread calls the functions below
  double decode_frame(const Frame & frame);
  // void display_decoded_frame(VideoDisplay & display); 
  void display_decoded_frame(VideoDisplay & display); 

  // NVDEC related
  CUcontext cuContext = NULL;
  OutputFormat eOutputFormat = native;
  // OutputFormat eOutputFormat = bgra;
  int iGpu = 0;
  uint8_t *pFrame;
  NvDecoder *pdec = NULL;
  const char *szTail = "\xe0\x00\x00\x00\x01\xce\x8c\x4d\x9d\x10\x8e\x25\xe9\xfe";
  
  std::unique_ptr<uint8_t[]> pRgbFrame; 
  int nRgbFramePitch = 0;
  int nRgbFrameSize = 0;
  int nFrameToDisplay_ = 0;
  CUdeviceptr dpRgbFrame = 0;

  // main function for the worker thread
  void worker_main();

};



#endif /* DECODER_HH */