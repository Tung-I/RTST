#include <sys/sysinfo.h>
#include <cassert>
#include <iostream>
#include <stdexcept>
#include <algorithm>

#include "HWDecoder.hh"
#include "exception.hh"
#include "conversion.hh"
#include "image.hh"
#include "timestamp.hh"


Frame::Frame(const uint32_t frame_id,
             const FrameType frame_type,
             const uint16_t frag_cnt)
  : id_(frame_id), type_(frame_type), frags_(frag_cnt), null_frags_(frag_cnt)
{
  if (frag_cnt == 0) {
    throw runtime_error("frame cannot have zero fragments");
  }
}

bool Frame::has_frag(const uint16_t frag_id) const
{
  return frags_.at(frag_id).has_value();
}

FrameDatagram & Frame::get_frag(const uint16_t frag_id)
{
  return frags_.at(frag_id).value();
}

const FrameDatagram & Frame::get_frag(const uint16_t frag_id) const
{
  return frags_.at(frag_id).value();
}

optional<size_t> Frame::frame_size() const
{
  if (not complete()) {
    return nullopt;
  }

  return frame_size_;
}

void Frame::validate_datagram(const FrameDatagram & datagram) const
{
  if (datagram.frame_id != id_ or
      datagram.frame_type != type_ or
      datagram.frag_id >= frags_.size() or
      datagram.frag_cnt != frags_.size()) {
    throw runtime_error("unable to insert an incompatible datagram");
  }
}

void Frame::insert_frag(const FrameDatagram & datagram)
{
  validate_datagram(datagram);

  // insert only if the datagram does not exist yet
  if (not frags_[datagram.frag_id]) {
    frame_size_ += datagram.payload.size();
    null_frags_--;
    frags_[datagram.frag_id] = datagram;
  }
}

void Frame::insert_frag(FrameDatagram && datagram)
{
  validate_datagram(datagram);

  // insert only if the datagram does not exist yet
  if (not frags_[datagram.frag_id]) {
    frame_size_ += datagram.payload.size();
    null_frags_--;
    frags_[datagram.frag_id] = std::move(datagram);
  }
}

HWDecoder::HWDecoder(const uint16_t display_width,
                 const uint16_t display_height,
                 const int lazy_level,
                 const string & output_path)
  : display_width_(display_width), display_height_(display_height),
    lazy_level_(), output_fd_(), decoder_epoch_(std::chrono::steady_clock::now())
{
  // validate lazy level
  if (lazy_level < DECODE_DISPLAY or lazy_level > NO_DECODE_DISPLAY) {
    throw runtime_error("Invalid lazy level: " + to_string(lazy_level));
  }
  lazy_level_ = static_cast<LazyLevel>(lazy_level);

  // both main and worker threads start from the same time for stats output
  last_stats_time_ = decoder_epoch_;

  // open the output file
  if (not output_path.empty()) {
    output_fd_ = FileDescriptor(check_syscall(
        open(output_path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644)));
  }

  // start the worker thread only if we are going to decode or display frames
  if (lazy_level <= DECODE_ONLY) {
    worker_ = thread(&HWDecoder::worker_main, this);  // thread(a pointer to member, the object, argument)
    cerr << "Spawned a new thread for decoding and displaying frames" << endl;
  }
}

bool HWDecoder::add_datagram_common(const FrameDatagram & datagram)
{
  const auto frame_id = datagram.frame_id;
  const auto frame_type = datagram.frame_type;
  const auto frag_cnt = datagram.frag_cnt;

  // ignore any datagrams from the old frames
  if (frame_id < next_frame_) {
    return false;
  }

  if (not frame_buf_.count(frame_id)) {
    // initialize a Frame instance for frame 'frame_id'
    frame_buf_.emplace(piecewise_construct, 
                       forward_as_tuple(frame_id),
                       forward_as_tuple(frame_id, frame_type, frag_cnt)); 
  }

  return true;
}

void HWDecoder::add_datagram(const FrameDatagram & datagram)
{
  if (not add_datagram_common(datagram)) {
    return;
  }

  // copy the fragment into the frame
  frame_buf_.at(datagram.frame_id).insert_frag(datagram);
}

void HWDecoder::add_datagram(FrameDatagram && datagram)
{
  if (not add_datagram_common(datagram)) {
    return;
  }

  // move the fragment into the frame
  frame_buf_.at(datagram.frame_id).insert_frag(std::move(datagram));
}

bool HWDecoder::next_frame_complete()
{
  {
    // if the frame is in the buffer and all of its fragments have been received
    auto it = as_const(frame_buf_).find(next_frame_);
    if (it != frame_buf_.end() and it->second.complete()) {
      return true;
    }
  }

  // seek forward if a key frame in the future is already complete
  for (auto it = frame_buf_.rbegin(); it != frame_buf_.rend(); it++) {
    const auto frame_id = it->first;
    const auto & frame = it->second;

    // found a complete key frame ahead of next_frame_
    if (frame.type() == FrameType::KEY and frame.complete()) {
      assert(frame_id > next_frame_);

      // set next_frame_ to frame_id and clean up old frames
      const auto frame_diff = frame_id - next_frame_;
      advance_next_frame(frame_diff);

      LOG(LogLevel::WARNING) << endl << "* Recovery: skipped " << frame_diff
           << " frames ahead to key frame " << frame_id << endl;

      return true;
    }
  }

  return false;
}

void HWDecoder::consume_next_frame()
{
  Frame & frame = frame_buf_.at(next_frame_);
  if (not frame.complete()) {
    throw runtime_error("next frame must be complete before consuming it");
  }

  // found a decodable frame; update (and output) stats
  num_decodable_frames_++;
  const size_t frame_size = frame.frame_size().value();
  total_decodable_frame_size_ += frame_size;
  // output stats 
  const auto stats_now = std::chrono::steady_clock::now();
  while (stats_now >= last_stats_time_ + 1s) {
    LOG(LogLevel::INFO) << "Decodable frames in the last ~1s: "
         << num_decodable_frames_;

    const double diff_ms = std::chrono::duration<double, milli>(
                           stats_now - last_stats_time_).count();
    if (diff_ms > 0) {
      LOG(LogLevel::INFO) << "  - Bitrate (kbps): "
           << double_to_string(total_decodable_frame_size_ * 8 / diff_ms);
    }

    // reset stats
    num_decodable_frames_ = 0;
    total_decodable_frame_size_ = 0;
    last_stats_time_ += 1s;
  }

  if (lazy_level_ <= DECODE_ONLY) {
    // dispatch the frame to worker thread
    {
      lock_guard<mutex> lock(mtx_);
      shared_queue_.emplace_back(std::move(frame));
    } // release the lock before notifying the worker thread

    // notify worker thread
    cv_.notify_one();
  } else {

  }

  // move onto the next frame
  advance_next_frame();
}

void HWDecoder::advance_next_frame(const unsigned int n)
{
  next_frame_ += n;
  clean_up_to(next_frame_);
}

void HWDecoder::clean_up_to(const uint32_t frontier)
{
  for (auto it = frame_buf_.begin(); it != frame_buf_.end(); ) {
    if (it->first >= frontier) {
      break;
    }

    it = frame_buf_.erase(it);
  }
}

double HWDecoder::decode_frame(const Frame & frame)
{
  const auto decode_start = std::chrono::steady_clock::now();

  if (not frame.complete()) {
    throw runtime_error("frame must be complete before decoding");
  }

  static constexpr size_t MAX_DECODING_BUF = 3000000; 
  static vector<uint8_t> decode_buf(MAX_DECODING_BUF);
  uint8_t * buf_ptr = decode_buf.data();
  const uint8_t * const buf_end = buf_ptr + decode_buf.size();


  size_t total_payload_size = 0;

  for (const auto & datagram : frame.frags()) {
    const string & payload = datagram.value().payload;
    if (buf_ptr + payload.size() >= buf_end) {
      throw runtime_error("frame size exceeds max decoding buffer size");
    }

    memcpy(buf_ptr, payload.data(), payload.size());

    total_payload_size += payload.size();

    buf_ptr += payload.size();
  }

  const size_t frame_size = buf_ptr - decode_buf.data(); 
  int nFrameReturned = pdec->Decode(decode_buf.data(), frame_size, CUVID_PKT_ENDOFPICTURE);
  nFrameToDisplay_ += nFrameReturned;
  const auto decode_end = std::chrono::steady_clock::now();
  return std::chrono::duration<double, milli>(decode_end - decode_start).count();
}

void HWDecoder::display_decoded_frame(VideoDisplay & display)
{

  // display the decoded frame stored in 'context_'
  while (nFrameToDisplay_ > 0) {
    if (nFrameToDisplay_ > 1) {
      throw runtime_error("Multiple frames were decoded at once");
    }

    NV12Image * pNV12_image = new NV12Image(display_width_, display_height_);
    pFrame = pdec->GetFrame();
    // int frame_size = pdec->GetFrameSize();
    pNV12_image->store_nv12_frame(pFrame, pdec->GetFrameSize());
    // construct a temporary RawImage that does not own the raw_img
    display.show_frame(*pNV12_image);
    
    nFrameToDisplay_--;
  }
}

void HWDecoder::worker_main()
{

  // worker does nothing if not decode or display
  if (lazy_level_ == NO_DECODE_DISPLAY) {
    return;
  }

  // Check cuda device
  ck(cuInit(0));
  int nGpu = 0;
  ck(cuDeviceGetCount(&nGpu));
  if (iGpu < 0 || iGpu >= nGpu) {
      std::cout << "GPU ordinal out of range. Should be within [" << 0 << ", " << nGpu - 1 << "]" << std::endl;
      exit(1);

  }
  CUdevice cuDevice = 0;
  ck(cuDeviceGet(&cuDevice, iGpu));
  char szDeviceName[80];
  ck(cuDeviceGetName(szDeviceName, sizeof(szDeviceName), cuDevice));
  std::cout << "GPU in use: " << szDeviceName << std::endl;

  // Create decoder interface  
  ck(cuCtxCreate(&cuContext, 0, cuDevice));
  // bool force_zero_latency = true;
  bool force_zero_latency = false;
  pdec = new NvDecoder(cuContext, eOutputFormat != native, cudaVideoCodec_HEVC, 
    true, false, NULL, NULL, false, 0, 0, 1000, force_zero_latency);

  // Create video displayer
  unique_ptr<VideoDisplay> display;
  if (lazy_level_ == DECODE_DISPLAY) {
    // display = make_unique<VideoDisplay>(display_width_, display_height_);
    display = make_unique<VideoDisplay>(display_width_, display_height_);
  }

  // Local queue of frames
  deque<Frame> local_queue;

  // Stats maintained by the worker thread
  unsigned int num_decoded_frames = 0;
  double total_decode_time_ms = 0.0;
  double max_decode_time_ms = 0.0;
  auto last_stats_time = decoder_epoch_;

  while (true) {
    if (display and display->signal_quit()) {
      display.reset(nullptr);
    }

    {  
      unique_lock<mutex> lock(mtx_);
      cv_.wait(lock, [this] { return not shared_queue_.empty(); }); // wait until the queue is not empty

      while (not shared_queue_.empty()) { // worker owns the lock after wait and should copy shared queue quickly
        local_queue.emplace_back(std::move(shared_queue_.front()));
        shared_queue_.pop_front();
      }
    } // worker releases the lock 

    while (not local_queue.empty()) {
      const Frame & frame = local_queue.front();
      const double decode_time_ms = decode_frame(frame);

      if (output_fd_) {
        const auto frame_decoded_ts = timestamp_us();
        output_fd_->write(to_string(frame.id()) + "," +
                          to_string(frame.frame_size().value()) + "," +
                          to_string(frame_decoded_ts) + "," +
                          to_string(decode_time_ms) + "\n"
                          );
      }

      if (display) {
        display_decoded_frame(*display); 
      }


      local_queue.pop_front();

      // update stats
      num_decoded_frames++;
      total_decode_time_ms += decode_time_ms;
      max_decode_time_ms = max(max_decode_time_ms, decode_time_ms);

      // worker thread also outputs stats roughly every second
      const auto stats_now = std::chrono::steady_clock::now();
      while (stats_now >= last_stats_time + 1s) {  
        if (num_decoded_frames > 0) {
          LOG(LogLevel::INFO) << "Avg/Max decoding time (ms) of "
               << num_decoded_frames << " frames: "
               << double_to_string(total_decode_time_ms / num_decoded_frames)
               << "/" << double_to_string(max_decode_time_ms);
        }

        // reset stats
        num_decoded_frames = 0;
        total_decode_time_ms = 0.0;
        max_decode_time_ms = 0.0;
        last_stats_time += 1s;
      }
    }
  }

}