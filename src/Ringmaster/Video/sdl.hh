#ifndef DISPLAY_HH
#define DISPLAY_HH

extern "C" {
#include <SDL2/SDL.h>
}

#include <memory>
#include "image.hh"

class VideoDisplay
{
public:
  VideoDisplay(const uint16_t display_width, const uint16_t display_height);
  ~VideoDisplay();

  // display a frame
  void show_frame(const RawImage & raw_img);
  void show_frame(const NV12Image & raw_img);

  // if signaled to quit
  bool signal_quit();

  // forbid copy and move operators
  VideoDisplay(const VideoDisplay & other) = delete;
  const VideoDisplay & operator=(const VideoDisplay & other) = delete;
  VideoDisplay(VideoDisplay && other) = delete;
  VideoDisplay & operator=(VideoDisplay && other) = delete;

private:
  uint16_t display_width_;
  uint16_t display_height_;

  SDL_Window * window_ {nullptr};
  SDL_Renderer * renderer_ {nullptr};
  SDL_Texture * texture_ {nullptr};
  std::unique_ptr<SDL_Event> event_ {nullptr};
};

class BGRAVideoDisplay
{
public:
  BGRAVideoDisplay(const uint16_t display_width, const uint16_t display_height);
  ~BGRAVideoDisplay();

  // display a frame
  void show_frame(const std::unique_ptr<uint8_t[]> &bgra_frame);

  // if signaled to quit
  bool signal_quit();

  // forbid copy and move operators
  BGRAVideoDisplay(const BGRAVideoDisplay & other) = delete;
  const BGRAVideoDisplay & operator=(const BGRAVideoDisplay & other) = delete;
  BGRAVideoDisplay(BGRAVideoDisplay && other) = delete;
  BGRAVideoDisplay & operator=(BGRAVideoDisplay && other) = delete;

private:
  uint16_t display_width_;
  uint16_t display_height_;

  SDL_Window * window_ {nullptr};
  SDL_Renderer * renderer_ {nullptr};
  SDL_Texture * texture_ {nullptr};
  std::unique_ptr<SDL_Event> event_ {nullptr};
};


#endif /* DISPLAY_HH */
