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
include AppDecode/AppDecLowLatency/CMakeFiles/AppDecLowLatency.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include AppDecode/AppDecLowLatency/CMakeFiles/AppDecLowLatency.dir/compiler_depend.make

# Include the progress variables for this target.
include AppDecode/AppDecLowLatency/CMakeFiles/AppDecLowLatency.dir/progress.make

# Include the compile flags for this target's objects.
include AppDecode/AppDecLowLatency/CMakeFiles/AppDecLowLatency.dir/flags.make

AppDecode/AppDecLowLatency/CMakeFiles/AppDecLowLatency.dir/AppDecLowLatency.cpp.o: AppDecode/AppDecLowLatency/CMakeFiles/AppDecLowLatency.dir/flags.make
AppDecode/AppDecLowLatency/CMakeFiles/AppDecLowLatency.dir/AppDecLowLatency.cpp.o: ../AppDecode/AppDecLowLatency/AppDecLowLatency.cpp
AppDecode/AppDecLowLatency/CMakeFiles/AppDecLowLatency.dir/AppDecLowLatency.cpp.o: AppDecode/AppDecLowLatency/CMakeFiles/AppDecLowLatency.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tungi/Video_Codec_SDK_12.1.14/Samples/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object AppDecode/AppDecLowLatency/CMakeFiles/AppDecLowLatency.dir/AppDecLowLatency.cpp.o"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppDecode/AppDecLowLatency && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT AppDecode/AppDecLowLatency/CMakeFiles/AppDecLowLatency.dir/AppDecLowLatency.cpp.o -MF CMakeFiles/AppDecLowLatency.dir/AppDecLowLatency.cpp.o.d -o CMakeFiles/AppDecLowLatency.dir/AppDecLowLatency.cpp.o -c /home/tungi/Video_Codec_SDK_12.1.14/Samples/AppDecode/AppDecLowLatency/AppDecLowLatency.cpp

AppDecode/AppDecLowLatency/CMakeFiles/AppDecLowLatency.dir/AppDecLowLatency.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AppDecLowLatency.dir/AppDecLowLatency.cpp.i"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppDecode/AppDecLowLatency && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tungi/Video_Codec_SDK_12.1.14/Samples/AppDecode/AppDecLowLatency/AppDecLowLatency.cpp > CMakeFiles/AppDecLowLatency.dir/AppDecLowLatency.cpp.i

AppDecode/AppDecLowLatency/CMakeFiles/AppDecLowLatency.dir/AppDecLowLatency.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AppDecLowLatency.dir/AppDecLowLatency.cpp.s"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppDecode/AppDecLowLatency && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tungi/Video_Codec_SDK_12.1.14/Samples/AppDecode/AppDecLowLatency/AppDecLowLatency.cpp -o CMakeFiles/AppDecLowLatency.dir/AppDecLowLatency.cpp.s

AppDecode/AppDecLowLatency/CMakeFiles/AppDecLowLatency.dir/__/__/NvCodec/NvDecoder/NvDecoder.cpp.o: AppDecode/AppDecLowLatency/CMakeFiles/AppDecLowLatency.dir/flags.make
AppDecode/AppDecLowLatency/CMakeFiles/AppDecLowLatency.dir/__/__/NvCodec/NvDecoder/NvDecoder.cpp.o: ../NvCodec/NvDecoder/NvDecoder.cpp
AppDecode/AppDecLowLatency/CMakeFiles/AppDecLowLatency.dir/__/__/NvCodec/NvDecoder/NvDecoder.cpp.o: AppDecode/AppDecLowLatency/CMakeFiles/AppDecLowLatency.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tungi/Video_Codec_SDK_12.1.14/Samples/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object AppDecode/AppDecLowLatency/CMakeFiles/AppDecLowLatency.dir/__/__/NvCodec/NvDecoder/NvDecoder.cpp.o"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppDecode/AppDecLowLatency && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT AppDecode/AppDecLowLatency/CMakeFiles/AppDecLowLatency.dir/__/__/NvCodec/NvDecoder/NvDecoder.cpp.o -MF CMakeFiles/AppDecLowLatency.dir/__/__/NvCodec/NvDecoder/NvDecoder.cpp.o.d -o CMakeFiles/AppDecLowLatency.dir/__/__/NvCodec/NvDecoder/NvDecoder.cpp.o -c /home/tungi/Video_Codec_SDK_12.1.14/Samples/NvCodec/NvDecoder/NvDecoder.cpp

AppDecode/AppDecLowLatency/CMakeFiles/AppDecLowLatency.dir/__/__/NvCodec/NvDecoder/NvDecoder.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AppDecLowLatency.dir/__/__/NvCodec/NvDecoder/NvDecoder.cpp.i"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppDecode/AppDecLowLatency && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tungi/Video_Codec_SDK_12.1.14/Samples/NvCodec/NvDecoder/NvDecoder.cpp > CMakeFiles/AppDecLowLatency.dir/__/__/NvCodec/NvDecoder/NvDecoder.cpp.i

AppDecode/AppDecLowLatency/CMakeFiles/AppDecLowLatency.dir/__/__/NvCodec/NvDecoder/NvDecoder.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AppDecLowLatency.dir/__/__/NvCodec/NvDecoder/NvDecoder.cpp.s"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppDecode/AppDecLowLatency && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tungi/Video_Codec_SDK_12.1.14/Samples/NvCodec/NvDecoder/NvDecoder.cpp -o CMakeFiles/AppDecLowLatency.dir/__/__/NvCodec/NvDecoder/NvDecoder.cpp.s

# Object files for target AppDecLowLatency
AppDecLowLatency_OBJECTS = \
"CMakeFiles/AppDecLowLatency.dir/AppDecLowLatency.cpp.o" \
"CMakeFiles/AppDecLowLatency.dir/__/__/NvCodec/NvDecoder/NvDecoder.cpp.o"

# External object files for target AppDecLowLatency
AppDecLowLatency_EXTERNAL_OBJECTS =

AppDecode/AppDecLowLatency/AppDecLowLatency: AppDecode/AppDecLowLatency/CMakeFiles/AppDecLowLatency.dir/AppDecLowLatency.cpp.o
AppDecode/AppDecLowLatency/AppDecLowLatency: AppDecode/AppDecLowLatency/CMakeFiles/AppDecLowLatency.dir/__/__/NvCodec/NvDecoder/NvDecoder.cpp.o
AppDecode/AppDecLowLatency/AppDecLowLatency: AppDecode/AppDecLowLatency/CMakeFiles/AppDecLowLatency.dir/build.make
AppDecode/AppDecLowLatency/AppDecLowLatency: /usr/local/cuda/lib64/libcudart_static.a
AppDecode/AppDecLowLatency/AppDecLowLatency: /usr/lib/x86_64-linux-gnu/librt.a
AppDecode/AppDecLowLatency/AppDecLowLatency: /usr/lib/x86_64-linux-gnu/libcuda.so
AppDecode/AppDecLowLatency/AppDecLowLatency: /usr/lib/x86_64-linux-gnu/libnvidia-encode.so
AppDecode/AppDecLowLatency/AppDecLowLatency: /usr/lib/x86_64-linux-gnu/libnvcuvid.so
AppDecode/AppDecLowLatency/AppDecLowLatency: /usr/local/lib/libavcodec.so
AppDecode/AppDecLowLatency/AppDecLowLatency: /usr/local/lib/libavformat.so
AppDecode/AppDecLowLatency/AppDecLowLatency: /usr/local/lib/libavutil.so
AppDecode/AppDecLowLatency/AppDecLowLatency: /usr/local/lib/libswresample.so
AppDecode/AppDecLowLatency/AppDecLowLatency: /usr/lib/x86_64-linux-gnu/librt.a
AppDecode/AppDecLowLatency/AppDecLowLatency: /usr/lib/x86_64-linux-gnu/libcuda.so
AppDecode/AppDecLowLatency/AppDecLowLatency: /usr/lib/x86_64-linux-gnu/libnvidia-encode.so
AppDecode/AppDecLowLatency/AppDecLowLatency: /usr/lib/x86_64-linux-gnu/libnvcuvid.so
AppDecode/AppDecLowLatency/AppDecLowLatency: /usr/local/lib/libavcodec.so
AppDecode/AppDecLowLatency/AppDecLowLatency: /usr/local/lib/libavformat.so
AppDecode/AppDecLowLatency/AppDecLowLatency: /usr/local/lib/libavutil.so
AppDecode/AppDecLowLatency/AppDecLowLatency: /usr/local/lib/libswresample.so
AppDecode/AppDecLowLatency/AppDecLowLatency: AppDecode/AppDecLowLatency/CMakeFiles/AppDecLowLatency.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tungi/Video_Codec_SDK_12.1.14/Samples/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Linking CXX executable AppDecLowLatency"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppDecode/AppDecLowLatency && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/AppDecLowLatency.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
AppDecode/AppDecLowLatency/CMakeFiles/AppDecLowLatency.dir/build: AppDecode/AppDecLowLatency/AppDecLowLatency
.PHONY : AppDecode/AppDecLowLatency/CMakeFiles/AppDecLowLatency.dir/build

AppDecode/AppDecLowLatency/CMakeFiles/AppDecLowLatency.dir/clean:
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppDecode/AppDecLowLatency && $(CMAKE_COMMAND) -P CMakeFiles/AppDecLowLatency.dir/cmake_clean.cmake
.PHONY : AppDecode/AppDecLowLatency/CMakeFiles/AppDecLowLatency.dir/clean

AppDecode/AppDecLowLatency/CMakeFiles/AppDecLowLatency.dir/depend:
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tungi/Video_Codec_SDK_12.1.14/Samples /home/tungi/Video_Codec_SDK_12.1.14/Samples/AppDecode/AppDecLowLatency /home/tungi/Video_Codec_SDK_12.1.14/Samples/build /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppDecode/AppDecLowLatency /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppDecode/AppDecLowLatency/CMakeFiles/AppDecLowLatency.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : AppDecode/AppDecLowLatency/CMakeFiles/AppDecLowLatency.dir/depend
