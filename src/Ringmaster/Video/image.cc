#include <cstring>
#include <stdexcept>
#include <thread>

#include "image.hh"

using namespace std;



NV12Image::NV12Image(const uint16_t display_width, const uint16_t display_height)
        : display_width_(display_width), display_height_(display_height) {
        // Allocate vpx_image in I420 format
        vpx_img_ = vpx_img_alloc(nullptr, VPX_IMG_FMT_I420, display_width, display_height, 1);
    }

NV12Image::~NV12Image() {
    if (vpx_img_ != nullptr) {
        vpx_img_free(vpx_img_);
    }
}

void NV12Image::store_nv12_frame(const uint8_t* nv12_data, size_t data_size) {
    if (data_size != display_width_ * display_height_ * 3 / 2) {
        throw std::runtime_error("Invalid NV12 data size");
    }

    // Copy Y plane directly
    const uint8_t* y_src = nv12_data;
    std::memcpy(vpx_img_->planes[VPX_PLANE_Y], y_src, display_width_ * display_height_);

    // Deinterleave UV plane
    const uint8_t* uv_src = nv12_data + display_width_ * display_height_;
    uint8_t* u_dst = vpx_img_->planes[VPX_PLANE_U];
    uint8_t* v_dst = vpx_img_->planes[VPX_PLANE_V];

    // std::cout << "display_width_: " << display_width_ << std::endl;
    // std::cout << "display_height_: " << display_height_ << std::endl;
    // std::cout << "Y stride: " << vpx_img_->stride[VPX_PLANE_Y] << std::endl;
    // std::cout << "U stride: " << vpx_img_->stride[VPX_PLANE_U] << std::endl;
    // std::cout << "V stride: " << vpx_img_->stride[VPX_PLANE_V] << std::endl;


    for (size_t i = 0; i < display_height_ / 2; ++i) {
        for (size_t j = 0; j < display_width_ / 2; ++j) {
            u_dst[i * vpx_img_->stride[VPX_PLANE_U] + j] = uv_src[i * display_width_ + 2 * j];
            v_dst[i * vpx_img_->stride[VPX_PLANE_V] + j] = uv_src[i * display_width_ + 2 * j + 1];
        }
    }
}
//////////////////////////////////////////////

// constructor that allocates and owns the vpx_image
RawImage::RawImage(const uint16_t display_width, const uint16_t display_height)
  : vpx_img_(vpx_img_alloc(nullptr, VPX_IMG_FMT_I420,
                           display_width, display_height, 1)),
    own_vpx_img_(true),
    display_width_(display_width),
    display_height_(display_height)
{}

// constructor with a non-owning pointer to vpx_image
RawImage::RawImage(vpx_image_t * const vpx_img)
  : vpx_img_(vpx_img),
    own_vpx_img_(false),
    display_width_(),
    display_height_()
{
  if (not vpx_img) {
    throw runtime_error("RawImage: unable to construct from a null vpx_img");
  }

  if (vpx_img->fmt != VPX_IMG_FMT_I420) {
    throw runtime_error("RawImage: only supports I420");
  }

  display_width_ = vpx_img->d_w;
  display_height_ = vpx_img->d_h;
}

RawImage::~RawImage()
{
  // free vpx_image only if the class owns it
  if (own_vpx_img_) {
    vpx_img_free(vpx_img_);
  }
}

void RawImage::copy_from_yuyv(const string_view src)
{
  // expects YUYV to have size of 2 * W * H
  if (src.size() != y_size() * 2) {
    throw runtime_error("RawImage: invalid YUYV size");
  }

  uint8_t * dst_y = y_plane();
  uint8_t * dst_u = u_plane();
  uint8_t * dst_v = v_plane();

  // copy Y plane
  const uint8_t * p = reinterpret_cast<const uint8_t *>(src.data());
  for (unsigned i = 0; i < y_size(); i++, p += 2) {
    *dst_y++ = *p;
  }

  // copy U and V planes
  p = reinterpret_cast<const uint8_t *>(src.data());
  for (unsigned i = 0; i < display_height_ / 2; i++, p += 2 * display_width_) {
    for (unsigned j = 0; j < display_width_ / 2; j++, p += 4) {
      *dst_u++ = p[1];
      *dst_v++ = p[3];
    }
  }
}

void RawImage::copy_y_from(const string_view src)
{ 
  // print out src.size() and y_size()
  std::cout << "src.size(): " << src.size() << std::endl;
  std::cout << "y_size(): " << y_size() << std::endl;
  if (src.size() != y_size()) {
    throw runtime_error("RawImage: invalid size for Y plane");
  }

  memcpy(y_plane(), src.data(), src.size());
}

void RawImage::copy_u_from(const string_view src)
{
  if (src.size() != uv_size()) {
    throw runtime_error("RawImage: invalid size for U plane");
  }

  memcpy(u_plane(), src.data(), src.size());
}

void RawImage::copy_v_from(const string_view src)
{
  if (src.size() != uv_size()) {
    throw runtime_error("RawImage: invalid size for V plane");
  }

  memcpy(v_plane(), src.data(), src.size());
}

//////////////////////////////////////////////////////////////////////////////////

CroppedImage::CroppedImage(uint16_t frame_width, uint16_t frame_height, uint16_t  width, uint16_t  height)
  : frame_width_(frame_width), frame_height_(frame_height), frame_img(frame_width, frame_height), cropped_img(width, height)
{}

void CroppedImage::crop(float viewpoint_x, float viewpoint_y, uint16_t width, uint16_t  height) {
    // Calculate the starting indices for cropping. Round to nearest integers.
    int start_x = round(viewpoint_x - width / 2.0f);
    int start_y = round(viewpoint_y - height / 2.0f);

    // Check for boundary conditions
    if (start_x < 0) start_x = 0;
    if (start_y < 0) start_y = 0;
    if (start_x + width > frame_width_) start_x = frame_width_ - width;
    if (start_y + height > frame_height_) start_y = frame_height_ - height;

    // For the Y plane
    uint8_t* dest_ptr_y = cropped_img.y_plane();
    uint8_t* src_ptr_y = frame_img.y_plane() + start_y * frame_img.y_stride() + start_x;
    for (int i = 0; i < height; ++i) {
        memcpy(dest_ptr_y, src_ptr_y, width);
        dest_ptr_y += cropped_img.y_stride();
        src_ptr_y += frame_img.y_stride();
    }

    // For the U and V planes
    uint8_t* dest_ptr_u = cropped_img.u_plane();
    uint8_t* src_ptr_u = frame_img.u_plane() + (start_y / 2) * frame_img.u_stride() + start_x / 2;
    uint8_t* dest_ptr_v = cropped_img.v_plane();
    uint8_t* src_ptr_v = frame_img.v_plane() + (start_y / 2) * frame_img.v_stride() + start_x / 2;
    for (int i = 0; i < height / 2; ++i) {
        memcpy(dest_ptr_u, src_ptr_u, width / 2);
        memcpy(dest_ptr_v, src_ptr_v, width / 2);
        dest_ptr_u += cropped_img.u_stride();
        src_ptr_u += frame_img.u_stride();
        dest_ptr_v += cropped_img.v_stride();
        src_ptr_v += frame_img.v_stride();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////

TiledImage::TiledImage(uint16_t frame_width, uint16_t frame_height, uint16_t n_row, uint16_t n_col)
  : frame_img(frame_width, frame_height), n_row_(n_row), n_col_(n_col), 
  frame_width_(frame_img.display_width()), frame_height_(frame_img.display_height()),
  tile_width_(frame_img.display_width() / n_col_), tile_height_(frame_img.display_height() / n_row_), 
  tiles([this](){
    std::vector<RawImage*> tmp;
    tmp.resize(n_row_ * n_col_);
    for (int i = 0; i < n_row_ * n_col_; ++i) {
      tmp[i] = new RawImage(tile_width_, tile_height_);
    }
    return tmp;
  }())
{}

void TiledImage::threaded_partition_tile(uint16_t row, uint16_t col) {
    RawImage &tile = *tiles[row * n_col_ + col];
    for(int i = 0; i < tile_height_; ++i) {
        for(int j = 0; j < tile_width_; ++j) {
            tile.y_plane()[i * tile.y_stride() + j] = frame_img.y_plane()[(row * tile_height_ + i) * frame_img.y_stride() + col * tile_width_ + j];
            if (i < tile_height_ / 2 && j < tile_width_ / 2) {
                tile.u_plane()[i * tile.u_stride() + j] = frame_img.u_plane()[(row * tile_height_ / 2 + i) * frame_img.u_stride() + col * tile_width_ / 2 + j];
                tile.v_plane()[i * tile.v_stride() + j] = frame_img.v_plane()[(row * tile_height_ / 2 + i) * frame_img.v_stride() + col * tile_width_ / 2 + j];
            }
        }
    }
}

void TiledImage::partition() {
    std::vector<std::thread> threads;
    for (int row = 0; row < n_row_; ++row) {
        for (int col = 0; col < n_col_; ++col) {
            threads.emplace_back(&TiledImage::threaded_partition_tile, this, row, col);
        }
    }
    for (auto& t : threads) {
        t.join();
    }
}

void TiledImage::threaded_merge_tile(uint16_t row, uint16_t col) {
    const RawImage &tile = *tiles[row * n_col_ + col];
    for(int i = 0; i < tile_height_; ++i) {
        for(int j = 0; j < tile_width_; ++j) {
            frame_img.y_plane()[(row * tile_height_ + i) * frame_img.y_stride() + col * tile_width_ + j] = tile.y_plane()[i * tile.y_stride() + j];
            if (i < tile_height_ / 2 && j < tile_width_ / 2) {
                frame_img.u_plane()[(row * tile_height_ / 2 + i) * frame_img.u_stride() + col * tile_width_ / 2 + j] = tile.u_plane()[i * tile.u_stride() + j];
                frame_img.v_plane()[(row * tile_height_ / 2 + i) * frame_img.v_stride() + col * tile_width_ / 2 + j] = tile.v_plane()[i * tile.v_stride() + j];
            }
        }
    }
}

void TiledImage::merge() {
    std::vector<std::thread> threads;
    for (int row = 0; row < n_row_; ++row) {
        for (int col = 0; col < n_col_; ++col) {
            threads.emplace_back(&TiledImage::threaded_merge_tile, this, row, col);
        }
    }
    for (auto& t : threads) {
        t.join();
    }
}

TiledImage::~TiledImage() {
  for(auto tile : tiles) {
    delete tile;
  }
}
