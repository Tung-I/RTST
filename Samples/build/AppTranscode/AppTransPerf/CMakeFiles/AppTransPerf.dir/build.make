# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.22

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/tungi/Video_Codec_SDK_12.1.14/Samples

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/tungi/Video_Codec_SDK_12.1.14/Samples/build

# Include any dependencies generated for this target.
include AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/compiler_depend.make

# Include the progress variables for this target.
include AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/progress.make

# Include the compile flags for this target's objects.
include AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/flags.make

AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/AppTransPerf.cpp.o: AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/flags.make
AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/AppTransPerf.cpp.o: ../AppTranscode/AppTransPerf/AppTransPerf.cpp
AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/AppTransPerf.cpp.o: AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tungi/Video_Codec_SDK_12.1.14/Samples/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/AppTransPerf.cpp.o"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppTranscode/AppTransPerf && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/AppTransPerf.cpp.o -MF CMakeFiles/AppTransPerf.dir/AppTransPerf.cpp.o.d -o CMakeFiles/AppTransPerf.dir/AppTransPerf.cpp.o -c /home/tungi/Video_Codec_SDK_12.1.14/Samples/AppTranscode/AppTransPerf/AppTransPerf.cpp

AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/AppTransPerf.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AppTransPerf.dir/AppTransPerf.cpp.i"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppTranscode/AppTransPerf && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tungi/Video_Codec_SDK_12.1.14/Samples/AppTranscode/AppTransPerf/AppTransPerf.cpp > CMakeFiles/AppTransPerf.dir/AppTransPerf.cpp.i

AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/AppTransPerf.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AppTransPerf.dir/AppTransPerf.cpp.s"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppTranscode/AppTransPerf && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tungi/Video_Codec_SDK_12.1.14/Samples/AppTranscode/AppTransPerf/AppTransPerf.cpp -o CMakeFiles/AppTransPerf.dir/AppTransPerf.cpp.s

AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.o: AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/flags.make
AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.o: ../NvCodec/NvEncoder/NvEncoder.cpp
AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.o: AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tungi/Video_Codec_SDK_12.1.14/Samples/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.o"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppTranscode/AppTransPerf && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.o -MF CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.o.d -o CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.o -c /home/tungi/Video_Codec_SDK_12.1.14/Samples/NvCodec/NvEncoder/NvEncoder.cpp

AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.i"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppTranscode/AppTransPerf && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tungi/Video_Codec_SDK_12.1.14/Samples/NvCodec/NvEncoder/NvEncoder.cpp > CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.i

AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.s"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppTranscode/AppTransPerf && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tungi/Video_Codec_SDK_12.1.14/Samples/NvCodec/NvEncoder/NvEncoder.cpp -o CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.s

AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvDecoder/NvDecoder.cpp.o: AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/flags.make
AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvDecoder/NvDecoder.cpp.o: ../NvCodec/NvDecoder/NvDecoder.cpp
AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvDecoder/NvDecoder.cpp.o: AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tungi/Video_Codec_SDK_12.1.14/Samples/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvDecoder/NvDecoder.cpp.o"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppTranscode/AppTransPerf && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvDecoder/NvDecoder.cpp.o -MF CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvDecoder/NvDecoder.cpp.o.d -o CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvDecoder/NvDecoder.cpp.o -c /home/tungi/Video_Codec_SDK_12.1.14/Samples/NvCodec/NvDecoder/NvDecoder.cpp

AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvDecoder/NvDecoder.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvDecoder/NvDecoder.cpp.i"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppTranscode/AppTransPerf && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tungi/Video_Codec_SDK_12.1.14/Samples/NvCodec/NvDecoder/NvDecoder.cpp > CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvDecoder/NvDecoder.cpp.i

AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvDecoder/NvDecoder.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvDecoder/NvDecoder.cpp.s"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppTranscode/AppTransPerf && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tungi/Video_Codec_SDK_12.1.14/Samples/NvCodec/NvDecoder/NvDecoder.cpp -o CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvDecoder/NvDecoder.cpp.s

AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvEncoder/NvEncoderCuda.cpp.o: AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/flags.make
AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvEncoder/NvEncoderCuda.cpp.o: ../NvCodec/NvEncoder/NvEncoderCuda.cpp
AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvEncoder/NvEncoderCuda.cpp.o: AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tungi/Video_Codec_SDK_12.1.14/Samples/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvEncoder/NvEncoderCuda.cpp.o"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppTranscode/AppTransPerf && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvEncoder/NvEncoderCuda.cpp.o -MF CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvEncoder/NvEncoderCuda.cpp.o.d -o CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvEncoder/NvEncoderCuda.cpp.o -c /home/tungi/Video_Codec_SDK_12.1.14/Samples/NvCodec/NvEncoder/NvEncoderCuda.cpp

AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvEncoder/NvEncoderCuda.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvEncoder/NvEncoderCuda.cpp.i"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppTranscode/AppTransPerf && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tungi/Video_Codec_SDK_12.1.14/Samples/NvCodec/NvEncoder/NvEncoderCuda.cpp > CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvEncoder/NvEncoderCuda.cpp.i

AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvEncoder/NvEncoderCuda.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvEncoder/NvEncoderCuda.cpp.s"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppTranscode/AppTransPerf && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tungi/Video_Codec_SDK_12.1.14/Samples/NvCodec/NvEncoder/NvEncoderCuda.cpp -o CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvEncoder/NvEncoderCuda.cpp.s

# Object files for target AppTransPerf
AppTransPerf_OBJECTS = \
"CMakeFiles/AppTransPerf.dir/AppTransPerf.cpp.o" \
"CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.o" \
"CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvDecoder/NvDecoder.cpp.o" \
"CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvEncoder/NvEncoderCuda.cpp.o"

# External object files for target AppTransPerf
AppTransPerf_EXTERNAL_OBJECTS =

AppTranscode/AppTransPerf/AppTransPerf: AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/AppTransPerf.cpp.o
AppTranscode/AppTransPerf/AppTransPerf: AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.o
AppTranscode/AppTransPerf/AppTransPerf: AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvDecoder/NvDecoder.cpp.o
AppTranscode/AppTransPerf/AppTransPerf: AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/__/__/NvCodec/NvEncoder/NvEncoderCuda.cpp.o
AppTranscode/AppTransPerf/AppTransPerf: AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/build.make
AppTranscode/AppTransPerf/AppTransPerf: /usr/local/cuda/lib64/libcudart_static.a
AppTranscode/AppTransPerf/AppTransPerf: /usr/lib/x86_64-linux-gnu/librt.a
AppTranscode/AppTransPerf/AppTransPerf: /usr/lib/x86_64-linux-gnu/libcuda.so
AppTranscode/AppTransPerf/AppTransPerf: /usr/lib/x86_64-linux-gnu/libnvidia-encode.so
AppTranscode/AppTransPerf/AppTransPerf: /usr/lib/x86_64-linux-gnu/libnvcuvid.so
AppTranscode/AppTransPerf/AppTransPerf: /usr/local/lib/libavcodec.so
AppTranscode/AppTransPerf/AppTransPerf: /usr/local/lib/libavformat.so
AppTranscode/AppTransPerf/AppTransPerf: /usr/local/lib/libavutil.so
AppTranscode/AppTransPerf/AppTransPerf: /usr/local/lib/libswresample.so
AppTranscode/AppTransPerf/AppTransPerf: /usr/lib/x86_64-linux-gnu/librt.a
AppTranscode/AppTransPerf/AppTransPerf: /usr/lib/x86_64-linux-gnu/libcuda.so
AppTranscode/AppTransPerf/AppTransPerf: /usr/lib/x86_64-linux-gnu/libnvidia-encode.so
AppTranscode/AppTransPerf/AppTransPerf: /usr/lib/x86_64-linux-gnu/libnvcuvid.so
AppTranscode/AppTransPerf/AppTransPerf: /usr/local/lib/libavcodec.so
AppTranscode/AppTransPerf/AppTransPerf: /usr/local/lib/libavformat.so
AppTranscode/AppTransPerf/AppTransPerf: /usr/local/lib/libavutil.so
AppTranscode/AppTransPerf/AppTransPerf: /usr/local/lib/libswresample.so
AppTranscode/AppTransPerf/AppTransPerf: AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tungi/Video_Codec_SDK_12.1.14/Samples/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable AppTransPerf"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppTranscode/AppTransPerf && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/AppTransPerf.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/build: AppTranscode/AppTransPerf/AppTransPerf
.PHONY : AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/build

AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/clean:
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppTranscode/AppTransPerf && $(CMAKE_COMMAND) -P CMakeFiles/AppTransPerf.dir/cmake_clean.cmake
.PHONY : AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/clean

AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/depend:
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tungi/Video_Codec_SDK_12.1.14/Samples /home/tungi/Video_Codec_SDK_12.1.14/Samples/AppTranscode/AppTransPerf /home/tungi/Video_Codec_SDK_12.1.14/Samples/build /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppTranscode/AppTransPerf /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : AppTranscode/AppTransPerf/CMakeFiles/AppTransPerf.dir/depend
