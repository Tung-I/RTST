#include <iostream>
#include <vector>

#include "yuv4mpeg.hh"
#include "exception.hh"
#include "conversion.hh"
#include "split.hh"

using namespace std;

YUV4MPEG::YUV4MPEG(const string & video_file_path,
                   const uint16_t display_width,
                   const uint16_t display_height,
                   const bool loop)
  : fd_(check_syscall(open(video_file_path.c_str(), O_RDONLY))),
    display_width_(display_width),
    display_height_(display_height),
    loop_(loop)
{
  const string y4m_signature = "YUV4MPEG2";
  if (fd_.readn(y4m_signature.size()) != y4m_signature) {
    throw runtime_error("invalid YUV4MPEG2 file signature");
  }

  const string & header = fd_.getline();
  const vector<string> & tokens = split(header, " ");

  for (const auto & token : tokens) {
    if (token.empty()) {
      continue;
    }

    switch (token[0]) {
      case 'W': // width
        if (strict_stoi(token.substr(1)) != display_width) {
          throw runtime_error("wrong YUV4MPEG2 frame width");
        }
        break;

      case 'H': // height
        if (strict_stoi(token.substr(1)) != display_height) {
          throw runtime_error("wrong YUV4MPEG2 frame height");
        }
        break;

      case 'C': // color space
        if (token.substr(0, 4) != "C420") {
          throw runtime_error("only YUV420 color space is supported");
        }
        break;

      default:
        break;
    }
  }
}

YUV4MPEG::~YUV4MPEG() {
  stop_buffering_ = true;
  cv_buffer_.notify_all();
  if (y_thread_.joinable()) y_thread_.join();
  if (u_thread_.joinable()) u_thread_.join();
  if (v_thread_.joinable()) v_thread_.join();
}

void YUV4MPEG::start_buffering() {
  y_thread_ = thread(&YUV4MPEG::buffer_thread, this, 'Y');
  u_thread_ = thread(&YUV4MPEG::buffer_thread, this, 'U');
  v_thread_ = thread(&YUV4MPEG::buffer_thread, this, 'V');
}

void YUV4MPEG::buffer_thread(char plane) {
  std::queue<std::vector<unsigned char>> *buffer;
  size_t size;

  switch(plane) {
    case 'Y':
      buffer = &y_buffer_;
      size = y_size();
      break;
    case 'U':
      buffer = &u_buffer_;
      size = uv_size();
      break;
    case 'V':
      buffer = &v_buffer_;
      size = uv_size();
      break;
    default:
      return;
  }

  while (!stop_buffering_) {
    std::unique_lock<std::mutex> lock(mtx_buffer_);
    cv_buffer_.wait(lock, [&]() { return buffer->size() < max_frames || stop_buffering_; });

    if (stop_buffering_) break;

    std::vector<unsigned char> data(size);
    // Assuming fd_.readn(size) fills the data vector
    memcpy(data, src.data(), src.size());
    fd_.readn(data.data(), size);
    buffer->push(data);
    
    cv_buffer_.notify_all();
  }
}


bool YUV4MPEG::read_frame(RawImage & raw_img) {

  if (raw_img.display_width() != display_width_ or
      raw_img.display_height() != display_height_) {
    throw runtime_error("YUV4MPEG: image dimensions don't match");
  }

  std::unique_lock<std::mutex> lock(mtx_buffer_);
  cv_buffer_.wait(lock, [&]() { 
    return !y_buffer_.empty() && !u_buffer_.empty() && !v_buffer_.empty();
  });

  std::vector<uint8_t> y_data = y_buffer_.front(); y_buffer_.pop();
  std::vector<uint8_t> u_data = u_buffer_.front(); u_buffer_.pop();
  std::vector<uint8_t> v_data = v_buffer_.front(); v_buffer_.pop();

  lock.unlock();
  cv_buffer_.notify_all();

  raw_img.copy_y_from(y_data.data());
  raw_img.copy_u_from(u_data.data());
  raw_img.copy_v_from(v_data.data());

  return true;
}

bool YUV4MPEG::read_frame(RawImage & raw_img)
{
  if (raw_img.display_width() != display_width_ or
      raw_img.display_height() != display_height_) {
    throw runtime_error("YUV4MPEG: image dimensions don't match");
  }

  string frame_header = fd_.getline();

  if (fd_.eof() and frame_header.empty()) {
    if (loop_) {
      // reset the file offset to the beginning and skip the header line
      fd_.reset_offset();
      fd_.getline();

      // should read "FRAME" again
      frame_header = fd_.getline();
    } else {
      // cannot read past end of file if not set to the 'loop' mode
      return false;
    }
  }

  if (frame_header.substr(0, 5) != "FRAME") {
    throw runtime_error("invalid YUV4MPEG2 input format");
  }

  // read Y, U, V planes in order
  raw_img.copy_y_from(fd_.readn(y_size()));
  raw_img.copy_u_from(fd_.readn(uv_size()));
  raw_img.copy_v_from(fd_.readn(uv_size()));

  return true;
}

// bool YUV4MPEG::read_frame(RawImage & raw_img)
// {

//   if (raw_img.display_width() != display_width_ or
//       raw_img.display_height() != display_height_) {
//     throw runtime_error("YUV4MPEG: image dimensions don't match");
//   }

//   string frame_header = fd_.getline();

//   if (fd_.eof() and frame_header.empty()) {
//     if (loop_) {
//       // reset the file offset to the beginning and skip the header line
//       fd_.reset_offset();
//       fd_.getline();

//       // should read "FRAME" again
//       frame_header = fd_.getline();
//     } else {
//       // cannot read past end of file if not set to the 'loop' mode
//       return false;
//     }
//   }

//   if (frame_header.substr(0, 5) != "FRAME") {
//     throw runtime_error("invalid YUV4MPEG2 input format");
//   }

//   std::lock_guard<std::mutex> lock(mtx_);


//   auto read_y = [&]() {
//     raw_img.copy_y_from(fd_.readn(y_size()));
//   };

//   auto read_u = [&]() {
//     raw_img.copy_u_from(fd_.readn(uv_size()));
//   };

//   auto read_v = [&]() {
//     raw_img.copy_v_from(fd_.readn(uv_size()));
//   };

//   std::thread thread_y(read_y);
//   std::thread thread_u(read_u);
//   std::thread thread_v(read_v);

//   thread_y.join();
//   thread_u.join();
//   thread_v.join();

//   return true;
// }
