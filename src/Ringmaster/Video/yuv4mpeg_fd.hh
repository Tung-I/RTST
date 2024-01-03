#ifndef YUV4MPEG_HH
#define YUV4MPEG_HH

#include <string>
#include <mutex>
#include <thread>
#include <future>

#include "file_descriptor.hh"
#include "video_input.hh"

class YUV4MPEG : public VideoInput
{
public:
  YUV4MPEG(const std::string & video_file_path,
           const uint16_t display_width,
           const uint16_t display_height,
           const bool loop = true);

  size_t frame_size() const { return display_width_ * display_height_ * 3 / 2; }
  size_t y_size() const { return display_width_ * display_height_; }
  size_t uv_size() const { return display_width_ * display_height_ / 4; }

  // try to fetch a video frame from video file into raw_img
  bool read_frame(RawImage & raw_img) override;

  // accessors
  FileDescriptor & fd_y() { return fd_y_; }
  FileDescriptor & fd_u() { return fd_u_; }
  FileDescriptor & fd_v() { return fd_v_; }
  uint16_t display_width() const override { return display_width_; }
  uint16_t display_height() const override { return display_height_; }

private:
  FileDescriptor fd_y_;
  FileDescriptor fd_u_;
  FileDescriptor fd_v_;
  uint16_t display_width_;
  uint16_t display_height_;

  // loop over the file infinitely
  bool loop_;

  // thread-safe
  std::mutex mtx_ {};
};

#endif /* YUV4MPEG_HH */
