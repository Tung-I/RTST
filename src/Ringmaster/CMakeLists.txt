# Copyright 2020-2022 NVIDIA Corporation.  All rights reserved.
#
# Please refer to the NVIDIA end user license agreement (EULA) associated
# with this source code for terms and conditions that govern your use of
# this software. Any use, reproduction, disclosure, or distribution of
# this software and related documentation outside the terms of the EULA
# is strictly prohibited.

# 3.7 is required for FindVulkan module support in CMake.
cmake_minimum_required(VERSION 3.7)

project(Ringmaster)

set(SENDER_SOURCES
 ${CMAKE_CURRENT_SOURCE_DIR}/sender.cpp
)

set(RECEIVER_SOURCES
 ${CMAKE_CURRENT_SOURCE_DIR}/receiver.cpp
)

set(NV_ENC_SOURCES
 ${NV_ENC_DIR}/NvEncoder.cpp
 ${NV_ENC_DIR}/NvEncoderCuda.cpp
 ${NV_DEC_DIR}/NvDecoder.cpp
 )

set(NV_ENC_HDRS
 ${NV_ENC_DIR}/NvEncoder.h
 ${NV_ENC_DIR}/NvEncoderCuda.h
 ${NV_DEC_DIR}/NvDecoder.h
 ${NVCODEC_PUBLIC_INTERFACE_DIR}/nvEncodeAPI.h
 ${NVCODEC_UTILS_DIR}/NvCodecUtils.h
 ${NVCODEC_UTILS_DIR}/NvEncoderCLIOptions.h
 ${NVCODEC_UTILS_DIR}/ColorSpace.h
 ${NVCODEC_UTILS_DIR}/FFmpegStreamer.h
 ${NVCODEC_UTILS_DIR}/FFmpegDemuxer.h
)

set (RM_SOURCES
#  ${RM_APP_DIR}/vp9_encoder.cc
#  ${RM_APP_DIR}/vp9_decoder.cc
 ${RM_APP_DIR}/HWEncoder.cc
 ${RM_APP_DIR}/HWDecoder.cc
 ${RM_APP_DIR}/protocol.cc
 ${RM_UTILS_DIR}/address.cc
 ${RM_UTILS_DIR}/conversion.cc
 ${RM_UTILS_DIR}/epoller.cc
 ${RM_UTILS_DIR}/file_descriptor.cc
 ${RM_UTILS_DIR}/mmap.cc
 ${RM_UTILS_DIR}/poller.cc
 ${RM_UTILS_DIR}/serialization.cc
 ${RM_UTILS_DIR}/socket.cc
 ${RM_UTILS_DIR}/split.cc
 ${RM_UTILS_DIR}/timerfd.cc
 ${RM_UTILS_DIR}/timestamp.cc
 ${RM_UTILS_DIR}/udp_socket.cc
 ${RM_VIDEO_DIR}/image.cc
 ${RM_VIDEO_DIR}/sdl.cc
 ${RM_VIDEO_DIR}/v4l2.cc
 ${RM_VIDEO_DIR}/yuv4mpeg.cc
)

set (RM_HDRS
#  ${RM_APP_DIR}/vp9_encoder.hh
#  ${RM_APP_DIR}/vp9_decoder.hh
 ${RM_APP_DIR}/HWEncoder.hh
 ${RM_APP_DIR}/HWDecoder.hh
 ${RM_APP_DIR}/protocol.hh
 ${RM_UTILS_DIR}/address.hh
 ${RM_UTILS_DIR}/conversion.hh
 ${RM_UTILS_DIR}/epoller.hh
 ${RM_UTILS_DIR}/exception.hh
 ${RM_UTILS_DIR}/file_descriptor.hh
 ${RM_UTILS_DIR}/mmap.hh
 ${RM_UTILS_DIR}/poller.hh
 ${RM_UTILS_DIR}/serialization.hh
 ${RM_UTILS_DIR}/socket.hh
 ${RM_UTILS_DIR}/split.hh
 ${RM_UTILS_DIR}/timerfd.hh
 ${RM_UTILS_DIR}/timestamp.hh
 ${RM_UTILS_DIR}/udp_socket.hh
 ${RM_VIDEO_DIR}/image.hh
 ${RM_VIDEO_DIR}/sdl.hh
 ${RM_VIDEO_DIR}/v4l2.hh
 ${RM_VIDEO_DIR}/video_input.hh
 ${RM_VIDEO_DIR}/yuv4mpeg.hh
)

set(NV_ENC_CUDA_UTILS
 ${NVCODEC_UTILS_DIR}/ColorSpace.cu
)

# source_group( "headers" FILES ${NV_ENC_HDRS} )
# source_group( "sources" FILES ${APP_SOURCES} ${NV_ENC_SOURCES} ${NV_ENC_CUDA_UTILS})

find_package(CUDA)  # provides variables like CUDA_INCLUDE_DIRS and CUDA_LIBRARIES which hold paths to the include directories and libraries 

# Configures CUDA compiler
set(CUDA_HOST_COMPILER ${CMAKE_CXX_COMPILER})
set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-gencode arch=compute_50,code=\"sm_50,compute_50\")
if (WIN32 AND CUDA_VERSION_MAJOR GREATER_EQUAL 11 AND CUDA_VERSION_MINOR GREATER_EQUAL 7)
    set(CUDA_NVCC_FLAGS ${CUDA_NVCC_FLAGS};-Xcompiler /MT)
endif()
if ( CMAKE_COMPILER_IS_GNUCC )
    if(NOT "${CUDA_NVCC_FLAGS}" MATCHES "-std=c\\+\\+11" )
        list(APPEND CUDA_NVCC_FLAGS -std=c++11)
    endif()
endif()

# Create an executable named "sender" from the listed sources
cuda_add_executable(sender ${SENDER_SOURCES} ${RM_SOURCES} ${NV_ENC_SOURCES} ${NV_ENC_CUDA_UTILS} ${RM_HDRS} ${NV_ENC_HDRS} ${NV_DEC_HDRS} ${NV_FFMPEG_HDRS})
cuda_add_executable(receiver ${RECEIVER_SOURCES} ${RM_SOURCES} ${NV_ENC_SOURCES} ${NV_ENC_CUDA_UTILS} ${RM_HDRS} ${NV_ENC_HDRS} ${NV_DEC_HDRS} ${NV_FFMPEG_HDRS})

# Sets properties on the target
set_target_properties(sender PROPERTIES
    CUDA_SEPARABLE_COMPILATION ON
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)
set_target_properties(receiver PROPERTIES
    CUDA_SEPARABLE_COMPILATION ON
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Specifies the directories the compiler should look in for source files 
target_include_directories(sender PUBLIC ${CUDA_INCLUDE_DIRS}
 ${NVCODEC_PUBLIC_INTERFACE_DIR}
 ${NVCODEC_UTILS_DIR}
 ${NV_FFMPEG_HDRS}
 ${NV_CODEC_DIR}
 ${RM_UTILS_DIR}
 ${RM_VIDEO_DIR}
)
target_include_directories(receiver PUBLIC ${CUDA_INCLUDE_DIRS}
 ${NVCODEC_PUBLIC_INTERFACE_DIR}
 ${NVCODEC_UTILS_DIR}
 ${NV_FFMPEG_HDRS}
 ${NV_CODEC_DIR}
 ${RM_UTILS_DIR}
 ${RM_VIDEO_DIR}
)

# Search for the required libraries in PKG_CONFIG_PATH
find_package(PkgConfig REQUIRED)
pkg_check_modules(VPX REQUIRED IMPORTED_TARGET vpx)
pkg_check_modules(SDL2 REQUIRED IMPORTED_TARGET sdl2)

# Links libraries to the target
target_link_libraries(sender ${CUDA_CUDA_LIBRARY} ${CMAKE_DL_LIBS} ${NVENCODEAPI_LIB} ${CUVID_LIB} ${AVCODEC_LIB}
 ${AVFORMAT_LIB} ${AVUTIL_LIB} ${SWRESAMPLE_LIB} PkgConfig::VPX PkgConfig::SDL2)
target_link_libraries(receiver ${CUDA_CUDA_LIBRARY} ${CMAKE_DL_LIBS} ${NVENCODEAPI_LIB} ${CUVID_LIB} ${AVCODEC_LIB}
${AVFORMAT_LIB} ${AVUTIL_LIB} ${SWRESAMPLE_LIB} PkgConfig::VPX PkgConfig::SDL2)

 # Specifies installation rules
install(TARGETS sender RUNTIME DESTINATION ${NVCODEC_SAMPLES_INSTALL_DIR})
install(TARGETS receiver RUNTIME DESTINATION ${NVCODEC_SAMPLES_INSTALL_DIR})
