# Real-Time Streaming Testbed (RTST)

RTST is designed to be an extensible real-time video streaming research platform, which emulates a headless peer-to-peer video call between two endpoints over UDP with application-level retransmission. The platform is implemented in C++, built on top of [Ringmaster](https://github.com/microsoft/ringmaster) and enhanced by [NVIDIA Video Codec SDK](https://developer.nvidia.com/video-codec-sdk). By leveraging NVENC and NVDEC, RTST can support 8K video encoding/decoding in real-time (> 30 fps).

The platform is still under progress with the goal of supporting more NVENC features and better performance measurements in the future (e.g., congestion control, network conditions). 

See below for the basic usage of RTST.

## Dependencies
- Testing environment: Ubuntu >20.04
- Follow the [Linux Configuration Requirements](https://docs.nvidia.com/video-technologies/video-codec-sdk/12.1/read-me/index.html) on the official NVIDIA Video Codec SDK webpage. Generally, the steps include:
    - Install NVIDIA GPU driver
    - Install CUDA Toolkit
    - Install FFmpeg([tutorial](https://trac.ffmpeg.org/wiki/CompilationGuide/Ubuntu))

## Building
Compile the `sender` and `receiver` with
```
cd src
./compile.sh
```

## Emulating a video call
A sample raw video file can be downloaded from (https://drive.google.com/file/d/1idsDXA44mVlYu3yiCTdIJKT7GLtF8WNz/view?usp=sharing), which has a resolution of 2048x1024 and a frame rate of 30 fps.

Next, go to `src/build` and execute the following commands in two terminals, respectively:
```
./sender 12345 ~/datasets/raw_video/2k_runner_pano_5s.yuv
./receiver 127.0.0.1 12345 2048 1024 --fps 30 --cbr 24000 --lazy 1 --streamtime 10
```
Regarding the above example, the sender compresses the raw video frames into bitstreams with an average bitrate of 24Mbps and the streaming will continue over 10 sec.