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
include AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/compiler_depend.make

# Include the progress variables for this target.
include AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/progress.make

# Include the compile flags for this target's objects.
include AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/flags.make

AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/AppEncGL.cpp.o: AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/flags.make
AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/AppEncGL.cpp.o: ../AppEncode/AppEncGL/AppEncGL.cpp
AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/AppEncGL.cpp.o: AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tungi/Video_Codec_SDK_12.1.14/Samples/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/AppEncGL.cpp.o"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppEncode/AppEncGL && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/AppEncGL.cpp.o -MF CMakeFiles/AppEncGL.dir/AppEncGL.cpp.o.d -o CMakeFiles/AppEncGL.dir/AppEncGL.cpp.o -c /home/tungi/Video_Codec_SDK_12.1.14/Samples/AppEncode/AppEncGL/AppEncGL.cpp

AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/AppEncGL.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AppEncGL.dir/AppEncGL.cpp.i"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppEncode/AppEncGL && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tungi/Video_Codec_SDK_12.1.14/Samples/AppEncode/AppEncGL/AppEncGL.cpp > CMakeFiles/AppEncGL.dir/AppEncGL.cpp.i

AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/AppEncGL.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AppEncGL.dir/AppEncGL.cpp.s"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppEncode/AppEncGL && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tungi/Video_Codec_SDK_12.1.14/Samples/AppEncode/AppEncGL/AppEncGL.cpp -o CMakeFiles/AppEncGL.dir/AppEncGL.cpp.s

AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/GraphicsUtils.cpp.o: AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/flags.make
AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/GraphicsUtils.cpp.o: ../AppEncode/AppEncGL/GraphicsUtils.cpp
AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/GraphicsUtils.cpp.o: AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tungi/Video_Codec_SDK_12.1.14/Samples/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Building CXX object AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/GraphicsUtils.cpp.o"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppEncode/AppEncGL && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/GraphicsUtils.cpp.o -MF CMakeFiles/AppEncGL.dir/GraphicsUtils.cpp.o.d -o CMakeFiles/AppEncGL.dir/GraphicsUtils.cpp.o -c /home/tungi/Video_Codec_SDK_12.1.14/Samples/AppEncode/AppEncGL/GraphicsUtils.cpp

AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/GraphicsUtils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AppEncGL.dir/GraphicsUtils.cpp.i"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppEncode/AppEncGL && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tungi/Video_Codec_SDK_12.1.14/Samples/AppEncode/AppEncGL/GraphicsUtils.cpp > CMakeFiles/AppEncGL.dir/GraphicsUtils.cpp.i

AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/GraphicsUtils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AppEncGL.dir/GraphicsUtils.cpp.s"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppEncode/AppEncGL && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tungi/Video_Codec_SDK_12.1.14/Samples/AppEncode/AppEncGL/GraphicsUtils.cpp -o CMakeFiles/AppEncGL.dir/GraphicsUtils.cpp.s

AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.o: AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/flags.make
AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.o: ../NvCodec/NvEncoder/NvEncoder.cpp
AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.o: AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tungi/Video_Codec_SDK_12.1.14/Samples/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_3) "Building CXX object AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.o"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppEncode/AppEncGL && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.o -MF CMakeFiles/AppEncGL.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.o.d -o CMakeFiles/AppEncGL.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.o -c /home/tungi/Video_Codec_SDK_12.1.14/Samples/NvCodec/NvEncoder/NvEncoder.cpp

AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AppEncGL.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.i"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppEncode/AppEncGL && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tungi/Video_Codec_SDK_12.1.14/Samples/NvCodec/NvEncoder/NvEncoder.cpp > CMakeFiles/AppEncGL.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.i

AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AppEncGL.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.s"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppEncode/AppEncGL && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tungi/Video_Codec_SDK_12.1.14/Samples/NvCodec/NvEncoder/NvEncoder.cpp -o CMakeFiles/AppEncGL.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.s

AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/__/__/NvCodec/NvEncoder/NvEncoderGL.cpp.o: AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/flags.make
AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/__/__/NvCodec/NvEncoder/NvEncoderGL.cpp.o: ../NvCodec/NvEncoder/NvEncoderGL.cpp
AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/__/__/NvCodec/NvEncoder/NvEncoderGL.cpp.o: AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/tungi/Video_Codec_SDK_12.1.14/Samples/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_4) "Building CXX object AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/__/__/NvCodec/NvEncoder/NvEncoderGL.cpp.o"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppEncode/AppEncGL && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/__/__/NvCodec/NvEncoder/NvEncoderGL.cpp.o -MF CMakeFiles/AppEncGL.dir/__/__/NvCodec/NvEncoder/NvEncoderGL.cpp.o.d -o CMakeFiles/AppEncGL.dir/__/__/NvCodec/NvEncoder/NvEncoderGL.cpp.o -c /home/tungi/Video_Codec_SDK_12.1.14/Samples/NvCodec/NvEncoder/NvEncoderGL.cpp

AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/__/__/NvCodec/NvEncoder/NvEncoderGL.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/AppEncGL.dir/__/__/NvCodec/NvEncoder/NvEncoderGL.cpp.i"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppEncode/AppEncGL && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/tungi/Video_Codec_SDK_12.1.14/Samples/NvCodec/NvEncoder/NvEncoderGL.cpp > CMakeFiles/AppEncGL.dir/__/__/NvCodec/NvEncoder/NvEncoderGL.cpp.i

AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/__/__/NvCodec/NvEncoder/NvEncoderGL.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/AppEncGL.dir/__/__/NvCodec/NvEncoder/NvEncoderGL.cpp.s"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppEncode/AppEncGL && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/tungi/Video_Codec_SDK_12.1.14/Samples/NvCodec/NvEncoder/NvEncoderGL.cpp -o CMakeFiles/AppEncGL.dir/__/__/NvCodec/NvEncoder/NvEncoderGL.cpp.s

# Object files for target AppEncGL
AppEncGL_OBJECTS = \
"CMakeFiles/AppEncGL.dir/AppEncGL.cpp.o" \
"CMakeFiles/AppEncGL.dir/GraphicsUtils.cpp.o" \
"CMakeFiles/AppEncGL.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.o" \
"CMakeFiles/AppEncGL.dir/__/__/NvCodec/NvEncoder/NvEncoderGL.cpp.o"

# External object files for target AppEncGL
AppEncGL_EXTERNAL_OBJECTS =

AppEncode/AppEncGL/AppEncGL: AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/AppEncGL.cpp.o
AppEncode/AppEncGL/AppEncGL: AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/GraphicsUtils.cpp.o
AppEncode/AppEncGL/AppEncGL: AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/__/__/NvCodec/NvEncoder/NvEncoder.cpp.o
AppEncode/AppEncGL/AppEncGL: AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/__/__/NvCodec/NvEncoder/NvEncoderGL.cpp.o
AppEncode/AppEncGL/AppEncGL: AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/build.make
AppEncode/AppEncGL/AppEncGL: /usr/lib/x86_64-linux-gnu/libnvidia-encode.so
AppEncode/AppEncGL/AppEncGL: /usr/lib/x86_64-linux-gnu/libnvcuvid.so
AppEncode/AppEncGL/AppEncGL: /usr/local/lib/libavcodec.so
AppEncode/AppEncGL/AppEncGL: /usr/local/lib/libavformat.so
AppEncode/AppEncGL/AppEncGL: /usr/local/lib/libavutil.so
AppEncode/AppEncGL/AppEncGL: /usr/local/lib/libswresample.so
AppEncode/AppEncGL/AppEncGL: /usr/lib/x86_64-linux-gnu/libglut.so
AppEncode/AppEncGL/AppEncGL: /usr/lib/x86_64-linux-gnu/libGLEW.so
AppEncode/AppEncGL/AppEncGL: /usr/lib/x86_64-linux-gnu/libX11.so
AppEncode/AppEncGL/AppEncGL: /usr/lib/x86_64-linux-gnu/libGL.so
AppEncode/AppEncGL/AppEncGL: /usr/lib/x86_64-linux-gnu/libEGL.so
AppEncode/AppEncGL/AppEncGL: AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/tungi/Video_Codec_SDK_12.1.14/Samples/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_5) "Linking CXX executable AppEncGL"
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppEncode/AppEncGL && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/AppEncGL.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/build: AppEncode/AppEncGL/AppEncGL
.PHONY : AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/build

AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/clean:
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppEncode/AppEncGL && $(CMAKE_COMMAND) -P CMakeFiles/AppEncGL.dir/cmake_clean.cmake
.PHONY : AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/clean

AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/depend:
	cd /home/tungi/Video_Codec_SDK_12.1.14/Samples/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/tungi/Video_Codec_SDK_12.1.14/Samples /home/tungi/Video_Codec_SDK_12.1.14/Samples/AppEncode/AppEncGL /home/tungi/Video_Codec_SDK_12.1.14/Samples/build /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppEncode/AppEncGL /home/tungi/Video_Codec_SDK_12.1.14/Samples/build/AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : AppEncode/AppEncGL/CMakeFiles/AppEncGL.dir/depend
