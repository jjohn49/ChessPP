# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.26

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
CMAKE_COMMAND = /app/extra/clion/bin/cmake/linux/x64/bin/cmake

# The command to remove a file.
RM = /app/extra/clion/bin/cmake/linux/x64/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/hugh/Projects/jj-git-repos/ChessPP

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/hugh/Projects/jj-git-repos/ChessPP/cmake-build-debug

# Include any dependencies generated for this target.
include _deps/sdl2_image-build/CMakeFiles/showanim.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include _deps/sdl2_image-build/CMakeFiles/showanim.dir/compiler_depend.make

# Include the progress variables for this target.
include _deps/sdl2_image-build/CMakeFiles/showanim.dir/progress.make

# Include the compile flags for this target's objects.
include _deps/sdl2_image-build/CMakeFiles/showanim.dir/flags.make

_deps/sdl2_image-build/CMakeFiles/showanim.dir/showanim.c.o: _deps/sdl2_image-build/CMakeFiles/showanim.dir/flags.make
_deps/sdl2_image-build/CMakeFiles/showanim.dir/showanim.c.o: _deps/sdl2_image-src/showanim.c
_deps/sdl2_image-build/CMakeFiles/showanim.dir/showanim.c.o: _deps/sdl2_image-build/CMakeFiles/showanim.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/hugh/Projects/jj-git-repos/ChessPP/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building C object _deps/sdl2_image-build/CMakeFiles/showanim.dir/showanim.c.o"
	cd /home/hugh/Projects/jj-git-repos/ChessPP/cmake-build-debug/_deps/sdl2_image-build && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -MD -MT _deps/sdl2_image-build/CMakeFiles/showanim.dir/showanim.c.o -MF CMakeFiles/showanim.dir/showanim.c.o.d -o CMakeFiles/showanim.dir/showanim.c.o -c /home/hugh/Projects/jj-git-repos/ChessPP/cmake-build-debug/_deps/sdl2_image-src/showanim.c

_deps/sdl2_image-build/CMakeFiles/showanim.dir/showanim.c.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing C source to CMakeFiles/showanim.dir/showanim.c.i"
	cd /home/hugh/Projects/jj-git-repos/ChessPP/cmake-build-debug/_deps/sdl2_image-build && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -E /home/hugh/Projects/jj-git-repos/ChessPP/cmake-build-debug/_deps/sdl2_image-src/showanim.c > CMakeFiles/showanim.dir/showanim.c.i

_deps/sdl2_image-build/CMakeFiles/showanim.dir/showanim.c.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling C source to assembly CMakeFiles/showanim.dir/showanim.c.s"
	cd /home/hugh/Projects/jj-git-repos/ChessPP/cmake-build-debug/_deps/sdl2_image-build && /usr/bin/cc $(C_DEFINES) $(C_INCLUDES) $(C_FLAGS) -S /home/hugh/Projects/jj-git-repos/ChessPP/cmake-build-debug/_deps/sdl2_image-src/showanim.c -o CMakeFiles/showanim.dir/showanim.c.s

# Object files for target showanim
showanim_OBJECTS = \
"CMakeFiles/showanim.dir/showanim.c.o"

# External object files for target showanim
showanim_EXTERNAL_OBJECTS =

_deps/sdl2_image-build/showanim: _deps/sdl2_image-build/CMakeFiles/showanim.dir/showanim.c.o
_deps/sdl2_image-build/showanim: _deps/sdl2_image-build/CMakeFiles/showanim.dir/build.make
_deps/sdl2_image-build/showanim: _deps/sdl2_image-build/libSDL2_imaged.a
_deps/sdl2_image-build/showanim: _deps/sdl2-build/libSDL2maind.a
_deps/sdl2_image-build/showanim: _deps/sdl2-build/libSDL2d.a
_deps/sdl2_image-build/showanim: _deps/sdl2_image-build/CMakeFiles/showanim.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/hugh/Projects/jj-git-repos/ChessPP/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking C executable showanim"
	cd /home/hugh/Projects/jj-git-repos/ChessPP/cmake-build-debug/_deps/sdl2_image-build && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/showanim.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
_deps/sdl2_image-build/CMakeFiles/showanim.dir/build: _deps/sdl2_image-build/showanim
.PHONY : _deps/sdl2_image-build/CMakeFiles/showanim.dir/build

_deps/sdl2_image-build/CMakeFiles/showanim.dir/clean:
	cd /home/hugh/Projects/jj-git-repos/ChessPP/cmake-build-debug/_deps/sdl2_image-build && $(CMAKE_COMMAND) -P CMakeFiles/showanim.dir/cmake_clean.cmake
.PHONY : _deps/sdl2_image-build/CMakeFiles/showanim.dir/clean

_deps/sdl2_image-build/CMakeFiles/showanim.dir/depend:
	cd /home/hugh/Projects/jj-git-repos/ChessPP/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hugh/Projects/jj-git-repos/ChessPP /home/hugh/Projects/jj-git-repos/ChessPP/cmake-build-debug/_deps/sdl2_image-src /home/hugh/Projects/jj-git-repos/ChessPP/cmake-build-debug /home/hugh/Projects/jj-git-repos/ChessPP/cmake-build-debug/_deps/sdl2_image-build /home/hugh/Projects/jj-git-repos/ChessPP/cmake-build-debug/_deps/sdl2_image-build/CMakeFiles/showanim.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : _deps/sdl2_image-build/CMakeFiles/showanim.dir/depend

