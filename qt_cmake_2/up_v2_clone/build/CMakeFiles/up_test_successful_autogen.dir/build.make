# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.10

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:


# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list


# Suppress display of executed commands.
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
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/sms/workspace/qt_cmake_2/up_v2_clone

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/sms/workspace/qt_cmake_2/up_v2_clone/build

# Utility rule file for up_test_successful_autogen.

# Include the progress variables for this target.
include CMakeFiles/up_test_successful_autogen.dir/progress.make

CMakeFiles/up_test_successful_autogen:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold --progress-dir=/home/sms/workspace/qt_cmake_2/up_v2_clone/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Automatic MOC, UIC and RCC for target up_test_successful"
	/usr/bin/cmake -E cmake_autogen /home/sms/workspace/qt_cmake_2/up_v2_clone/build/CMakeFiles/up_test_successful_autogen.dir ""

up_test_successful_autogen: CMakeFiles/up_test_successful_autogen
up_test_successful_autogen: CMakeFiles/up_test_successful_autogen.dir/build.make

.PHONY : up_test_successful_autogen

# Rule to build all files generated by this target.
CMakeFiles/up_test_successful_autogen.dir/build: up_test_successful_autogen

.PHONY : CMakeFiles/up_test_successful_autogen.dir/build

CMakeFiles/up_test_successful_autogen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/up_test_successful_autogen.dir/cmake_clean.cmake
.PHONY : CMakeFiles/up_test_successful_autogen.dir/clean

CMakeFiles/up_test_successful_autogen.dir/depend:
	cd /home/sms/workspace/qt_cmake_2/up_v2_clone/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/sms/workspace/qt_cmake_2/up_v2_clone /home/sms/workspace/qt_cmake_2/up_v2_clone /home/sms/workspace/qt_cmake_2/up_v2_clone/build /home/sms/workspace/qt_cmake_2/up_v2_clone/build /home/sms/workspace/qt_cmake_2/up_v2_clone/build/CMakeFiles/up_test_successful_autogen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/up_test_successful_autogen.dir/depend

