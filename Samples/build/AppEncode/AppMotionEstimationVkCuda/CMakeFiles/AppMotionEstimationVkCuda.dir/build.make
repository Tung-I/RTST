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
include AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/compiler_depend.make

# Include the progress variables for this target.
include AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/progress.make

# Include the compile flags for this target's objects.
include AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/flags.make

AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/AppMotionEstimationVkCuda.cpp.o: AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/flags.make
AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/AppMotionEstimationVkCuda.cpp.o: ../AppEncode/AppMotionEstimationVkCuda/AppMotionEstimationVkCuda.cpp
AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/AppMotionEstimationVkCuda.cpp.o: AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tungi/Video_Codec_SDK_12.1.14/Samples/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/AppMotionEstimationVkCuda.cpp.o"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppEncode/AppMotionEstimationVkCuda && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/AppMotionEstimationVkCuda.cpp.o -MF CMakeFiles/AppMotionEstimationVkCuda.dir/AppMotionEstimationVkCuda.cpp.o.d -o CMakeFiles/AppMotionEstimationVkCuda.dir/AppMotionEstimationVkCuda.cpp.o -c /home/tungi/Video_Codec_SDK_12.1.14/Samples/AppEncode/AppMotionEstimationVkCuda/AppMotionEstimationVkCuda.cpp

AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/AppMotionEstimationVkCuda.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AppMotionEstimationVkCuda.dir/AppMotionEstimationVkCuda.cpp.i"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppEncode/AppMotionEstimationVkCuda && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tungi/Video_Codec_SDK_12.1.14/Samples/AppEncode/AppMotionEstimationVkCuda/AppMotionEstimationVkCuda.cpp > CMakeFiles/AppMotionEstimationVkCuda.dir/AppMotionEstimationVkCuda.cpp.i

AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/AppMotionEstimationVkCuda.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AppMotionEstimationVkCuda.dir/AppMotionEstimationVkCuda.cpp.s"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppEncode/AppMotionEstimationVkCuda && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tungi/Video_Codec_SDK_12.1.14/Samples/AppEncode/AppMotionEstimationVkCuda/AppMotionEstimationVkCuda.cpp -o CMakeFiles/AppMotionEstimationVkCuda.dir/AppMotionEstimationVkCuda.cpp.s

AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/NvEnc.cpp.o: AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/flags.make
AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/NvEnc.cpp.o: ../AppEncode/AppMotionEstimationVkCuda/NvEnc.cpp
AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/NvEnc.cpp.o: AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tungi/Video_Codec_SDK_12.1.14/Samples/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/NvEnc.cpp.o"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppEncode/AppMotionEstimationVkCuda && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/NvEnc.cpp.o -MF CMakeFiles/AppMotionEstimationVkCuda.dir/NvEnc.cpp.o.d -o CMakeFiles/AppMotionEstimationVkCuda.dir/NvEnc.cpp.o -c /home/tungi/Video_Codec_SDK_12.1.14/Samples/AppEncode/AppMotionEstimationVkCuda/NvEnc.cpp

AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/NvEnc.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AppMotionEstimationVkCuda.dir/NvEnc.cpp.i"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppEncode/AppMotionEstimationVkCuda && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tungi/Video_Codec_SDK_12.1.14/Samples/AppEncode/AppMotionEstimationVkCuda/NvEnc.cpp > CMakeFiles/AppMotionEstimationVkCuda.dir/NvEnc.cpp.i

AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/NvEnc.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AppMotionEstimationVkCuda.dir/NvEnc.cpp.s"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppEncode/AppMotionEstimationVkCuda && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tungi/Video_Codec_SDK_12.1.14/Samples/AppEncode/AppMotionEstimationVkCuda/NvEnc.cpp -o CMakeFiles/AppMotionEstimationVkCuda.dir/NvEnc.cpp.s

AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/utility.cpp.o: AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/flags.make
AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/utility.cpp.o: ../AppEncode/AppMotionEstimationVkCuda/utility.cpp
AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/utility.cpp.o: AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tungi/Video_Codec_SDK_12.1.14/Samples/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/utility.cpp.o"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppEncode/AppMotionEstimationVkCuda && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/utility.cpp.o -MF CMakeFiles/AppMotionEstimationVkCuda.dir/utility.cpp.o.d -o CMakeFiles/AppMotionEstimationVkCuda.dir/utility.cpp.o -c /home/tungi/Video_Codec_SDK_12.1.14/Samples/AppEncode/AppMotionEstimationVkCuda/utility.cpp

AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/utility.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AppMotionEstimationVkCuda.dir/utility.cpp.i"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppEncode/AppMotionEstimationVkCuda && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tungi/Video_Codec_SDK_12.1.14/Samples/AppEncode/AppMotionEstimationVkCuda/utility.cpp > CMakeFiles/AppMotionEstimationVkCuda.dir/utility.cpp.i

AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/utility.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AppMotionEstimationVkCuda.dir/utility.cpp.s"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppEncode/AppMotionEstimationVkCuda && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tungi/Video_Codec_SDK_12.1.14/Samples/AppEncode/AppMotionEstimationVkCuda/utility.cpp -o CMakeFiles/AppMotionEstimationVkCuda.dir/utility.cpp.s

AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.o: AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/flags.make
AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.o: ../NvCodec/NvEncoder/NvEncoder.cpp
AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.o: AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tungi/Video_Codec_SDK_12.1.14/Samples/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.o"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppEncode/AppMotionEstimationVkCuda && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.o -MF CMakeFiles/AppMotionEstimationVkCuda.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.o.d -o CMakeFiles/AppMotionEstimationVkCuda.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.o -c /home/tungi/Video_Codec_SDK_12.1.14/Samples/NvCodec/NvEncoder/NvEncoder.cpp

AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AppMotionEstimationVkCuda.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.i"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppEncode/AppMotionEstimationVkCuda && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tungi/Video_Codec_SDK_12.1.14/Samples/NvCodec/NvEncoder/NvEncoder.cpp > CMakeFiles/AppMotionEstimationVkCuda.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.i

AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AppMotionEstimationVkCuda.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.s"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppEncode/AppMotionEstimationVkCuda && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tungi/Video_Codec_SDK_12.1.14/Samples/NvCodec/NvEncoder/NvEncoder.cpp -o CMakeFiles/AppMotionEstimationVkCuda.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.s

# Object files for target AppMotionEstimationVkCuda
AppMotionEstimationVkCuda_OBJECTS = \
"CMakeFiles/AppMotionEstimationVkCuda.dir/AppMotionEstimationVkCuda.cpp.o" \
"CMakeFiles/AppMotionEstimationVkCuda.dir/NvEnc.cpp.o" \
"CMakeFiles/AppMotionEstimationVkCuda.dir/utility.cpp.o" \
"CMakeFiles/AppMotionEstimationVkCuda.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.o"

# External object files for target AppMotionEstimationVkCuda
AppMotionEstimationVkCuda_EXTERNAL_OBJECTS =

AppEncode/AppMotionEstimationVkCuda/AppMotionEstimationVkCuda: AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/AppMotionEstimationVkCuda.cpp.o
AppEncode/AppMotionEstimationVkCuda/AppMotionEstimationVkCuda: AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/NvEnc.cpp.o
AppEncode/AppMotionEstimationVkCuda/AppMotionEstimationVkCuda: AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/utility.cpp.o
AppEncode/AppMotionEstimationVkCuda/AppMotionEstimationVkCuda: AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.o
AppEncode/AppMotionEstimationVkCuda/AppMotionEstimationVkCuda: AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/build.make
AppEncode/AppMotionEstimationVkCuda/AppMotionEstimationVkCuda: /usr/local/cuda/lib64/libcudart_static.a
AppEncode/AppMotionEstimationVkCuda/AppMotionEstimationVkCuda: /usr/lib/x86_64-linux-gnu/librt.a
AppEncode/AppMotionEstimationVkCuda/AppMotionEstimationVkCuda: /usr/lib/x86_64-linux-gnu/libcuda.so
AppEncode/AppMotionEstimationVkCuda/AppMotionEstimationVkCuda: /usr/lib/x86_64-linux-gnu/libnvidia-encode.so
AppEncode/AppMotionEstimationVkCuda/AppMotionEstimationVkCuda: /usr/lib/x86_64-linux-gnu/libnvcuvid.so
AppEncode/AppMotionEstimationVkCuda/AppMotionEstimationVkCuda: /usr/lib/x86_64-linux-gnu/libvulkan.so
AppEncode/AppMotionEstimationVkCuda/AppMotionEstimationVkCuda: /usr/lib/x86_64-linux-gnu/librt.a
AppEncode/AppMotionEstimationVkCuda/AppMotionEstimationVkCuda: /usr/lib/x86_64-linux-gnu/libcuda.so
AppEncode/AppMotionEstimationVkCuda/AppMotionEstimationVkCuda: /usr/lib/x86_64-linux-gnu/libnvidia-encode.so
AppEncode/AppMotionEstimationVkCuda/AppMotionEstimationVkCuda: /usr/lib/x86_64-linux-gnu/libnvcuvid.so
AppEncode/AppMotionEstimationVkCuda/AppMotionEstimationVkCuda: /usr/lib/x86_64-linux-gnu/libvulkan.so
AppEncode/AppMotionEstimationVkCuda/AppMotionEstimationVkCuda: AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tungi/Video_Codec_SDK_12.1.14/Samples/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable AppMotionEstimationVkCuda"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppEncode/AppMotionEstimationVkCuda && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/AppMotionEstimationVkCuda.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/build: AppEncode/AppMotionEstimationVkCuda/AppMotionEstimationVkCuda
.PHONY : AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/build

AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/clean:
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppEncode/AppMotionEstimationVkCuda && $(CMAKE_COMMAND) -P CMakeFiles/AppMotionEstimationVkCuda.dir/cmake_clean.cmake
.PHONY : AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/clean

AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/depend:
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tungi/Video_Codec_SDK_12.1.14/Samples /home/tungi/Video_Codec_SDK_12.1.14/Samples/AppEncode/AppMotionEstimationVkCuda /home/tungi/Video_Codec_SDK_12.1.14/Samples/build /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppEncode/AppMotionEstimationVkCuda /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : AppEncode/AppMotionEstimationVkCuda/CMakeFiles/AppMotionEstimationVkCuda.dir/depend

