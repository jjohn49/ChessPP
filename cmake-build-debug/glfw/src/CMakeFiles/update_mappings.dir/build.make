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

# Utility rule file for update_mappings.

# Include any custom commands dependencies for this target.
include glfw/src/CMakeFiles/update_mappings.dir/compiler_depend.make

# Include the progress variables for this target.
include glfw/src/CMakeFiles/update_mappings.dir/progress.make

glfw/src/CMakeFiles/update_mappings:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/hugh/Projects/jj-git-repos/ChessPP/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Updating gamepad mappings from upstream repository"
	cd /home/hugh/Projects/jj-git-repos/ChessPP/cmake-build-debug/glfw/src && /app/extra/clion/bin/cmake/linux/x64/bin/cmake -P /home/hugh/Projects/jj-git-repos/ChessPP/glfw/CMake/GenerateMappings.cmake mappings.h.in mappings.h

update_mappings: glfw/src/CMakeFiles/update_mappings
update_mappings: glfw/src/CMakeFiles/update_mappings.dir/build.make
.PHONY : update_mappings

# Rule to build all files generated by this target.
glfw/src/CMakeFiles/update_mappings.dir/build: update_mappings
.PHONY : glfw/src/CMakeFiles/update_mappings.dir/build

glfw/src/CMakeFiles/update_mappings.dir/clean:
	cd /home/hugh/Projects/jj-git-repos/ChessPP/cmake-build-debug/glfw/src && $(CMAKE_COMMAND) -P CMakeFiles/update_mappings.dir/cmake_clean.cmake
.PHONY : glfw/src/CMakeFiles/update_mappings.dir/clean

glfw/src/CMakeFiles/update_mappings.dir/depend:
	cd /home/hugh/Projects/jj-git-repos/ChessPP/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/hugh/Projects/jj-git-repos/ChessPP /home/hugh/Projects/jj-git-repos/ChessPP/glfw/src /home/hugh/Projects/jj-git-repos/ChessPP/cmake-build-debug /home/hugh/Projects/jj-git-repos/ChessPP/cmake-build-debug/glfw/src /home/hugh/Projects/jj-git-repos/ChessPP/cmake-build-debug/glfw/src/CMakeFiles/update_mappings.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : glfw/src/CMakeFiles/update_mappings.dir/depend
