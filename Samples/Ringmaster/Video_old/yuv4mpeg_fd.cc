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
  : fd_y_(check_syscall(open(video_file_path.c_str(), O_RDONLY))),
    fd_u_(check_syscall(open(video_file_path.c_str(), O_RDONLY))),
    fd_v_(check_syscall(open(video_file_path.c_str(), O_RDONLY))),
    display_width_(display_width),
    display_height_(display_height),
    loop_(loop)
{
  const string y4m_signature = "YUV4MPEG2";
  if (fd_y_.readn(y4m_signature.size()) != y4m_signature) {
    throw runtime_error("invalid YUV4MPEG2 file signature");
  }
  if (fd_u_.readn(y4m_signature.size()) != y4m_signature) {
    throw runtime_error("invalid YUV4MPEG2 file signature");
  }
  if (fd_v_.readn(y4m_signature.size()) != y4m_signature) {
    throw runtime_error("invalid YUV4MPEG2 file signature");
  }

  const string & header = fd_y_.getline();
  const string & header_u  = fd_u_.getline();
  const string & header_v  = fd_v_.getline();
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



bool YUV4MPEG::read_frame(RawImage & raw_img)
{

  if (raw_img.display_width() != display_width_ or
      raw_img.display_height() != display_height_) {
    throw runtime_error("YUV4MPEG: image dimensions don't match");
  }




  lock_guard<mutex> lock(mtx_);

  auto read_y = [&]() {
    string frame_header = fd_y_.getline();

    if (fd_y_.eof() and frame_header.empty()) {
      if (loop_) {
        // reset the file offset to the beginning and skip the header line
        fd_y_.reset_offset();
        fd_y_.getline();

        // should read "FRAME" again
        frame_header = fd_y_.getline();
      } else {
        // cannot read past end of file if not set to the 'loop' mode
        // return false;
      }
    }

    if (frame_header.substr(0, 5) != "FRAME") {
      throw runtime_error("invalid YUV4MPEG2 input format");
    }

    raw_img.copy_y_from(fd_y_.readn(y_size()));
  };

  auto read_u = [&]() {
    string frame_header = fd_u_.getline();

    if (fd_u_.eof() and frame_header.empty()) {
      if (loop_) {
        // reset the file offset to the beginning and skip the header line
        fd_u_.reset_offset();
        fd_u_.getline();

        // should read "FRAME" again
        frame_header = fd_u_.getline();
      } else {
        // cannot read past end of file if not set to the 'loop' mode
        // return false;
      }
    }

    if (frame_header.substr(0, 5) != "FRAME") {
      throw runtime_error("invalid YUV4MPEG2 input format");
    }

    fd_u_.seek(y_size(), SEEK_CUR);
    raw_img.copy_u_from(fd_u_.readn(uv_size()));
  };

  auto read_v = [&]() {
    string frame_header = fd_v_.getline();

    if (fd_v_.eof() and frame_header.empty()) {
      if (loop_) {
        // reset the file offset to the beginning and skip the header line
        fd_v_.reset_offset();
        fd_v_.getline();

        // should read "FRAME" again
        frame_header = fd_v_.getline();
      } else {
        // cannot read past end of file if not set to the 'loop' mode
        // return false;
      }
    }

    if (frame_header.substr(0, 5) != "FRAME") {
      throw runtime_error("invalid YUV4MPEG2 input format");
    }

    fd_v_.seek(y_size() + uv_size(), SEEK_CUR);
    raw_img.copy_u_from(fd_v_.readn(uv_size()));
  };

  thread thread_y(read_y);
  thread thread_u(read_u);
  thread thread_v(read_v);

  thread_y.join();
  thread_u.join();
  thread_v.join();


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

//   // read Y, U, V planes in order
//   raw_img.copy_y_from(fd_.readn(y_size()));
//   raw_img.copy_u_from(fd_.readn(uv_size()));
//   raw_img.copy_v_from(fd_.readn(uv_size()));

//   return true;
// }