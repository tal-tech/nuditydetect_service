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


# Produce verbose output by default.
VERBOSE = 1

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
CMAKE_SOURCE_DIR = /home/chenwen/Desktop/cpp/correct_det/src

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/chenwen/Desktop/cpp/correct_det/src

# Include any dependencies generated for this target.
include base/CMakeFiles/base_static.dir/depend.make

# Include the progress variables for this target.
include base/CMakeFiles/base_static.dir/progress.make

# Include the compile flags for this target's objects.
include base/CMakeFiles/base_static.dir/flags.make

base/CMakeFiles/base_static.dir/base_switches.cc.o: base/CMakeFiles/base_static.dir/flags.make
base/CMakeFiles/base_static.dir/base_switches.cc.o: base/base_switches.cc
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/chenwen/Desktop/cpp/correct_det/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object base/CMakeFiles/base_static.dir/base_switches.cc.o"
	cd /home/chenwen/Desktop/cpp/correct_det/src/base && /usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/base_static.dir/base_switches.cc.o -c /home/chenwen/Desktop/cpp/correct_det/src/base/base_switches.cc

base/CMakeFiles/base_static.dir/base_switches.cc.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/base_static.dir/base_switches.cc.i"
	cd /home/chenwen/Desktop/cpp/correct_det/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/chenwen/Desktop/cpp/correct_det/src/base/base_switches.cc > CMakeFiles/base_static.dir/base_switches.cc.i

base/CMakeFiles/base_static.dir/base_switches.cc.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/base_static.dir/base_switches.cc.s"
	cd /home/chenwen/Desktop/cpp/correct_det/src/base && /usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/chenwen/Desktop/cpp/correct_det/src/base/base_switches.cc -o CMakeFiles/base_static.dir/base_switches.cc.s

base/CMakeFiles/base_static.dir/base_switches.cc.o.requires:

.PHONY : base/CMakeFiles/base_static.dir/base_switches.cc.o.requires

base/CMakeFiles/base_static.dir/base_switches.cc.o.provides: base/CMakeFiles/base_static.dir/base_switches.cc.o.requires
	$(MAKE) -f base/CMakeFiles/base_static.dir/build.make base/CMakeFiles/base_static.dir/base_switches.cc.o.provides.build
.PHONY : base/CMakeFiles/base_static.dir/base_switches.cc.o.provides

base/CMakeFiles/base_static.dir/base_switches.cc.o.provides.build: base/CMakeFiles/base_static.dir/base_switches.cc.o


# Object files for target base_static
base_static_OBJECTS = \
"CMakeFiles/base_static.dir/base_switches.cc.o"

# External object files for target base_static
base_static_EXTERNAL_OBJECTS =

base/libbase_static.a: base/CMakeFiles/base_static.dir/base_switches.cc.o
base/libbase_static.a: base/CMakeFiles/base_static.dir/build.make
base/libbase_static.a: base/CMakeFiles/base_static.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/chenwen/Desktop/cpp/correct_det/src/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libbase_static.a"
	cd /home/chenwen/Desktop/cpp/correct_det/src/base && $(CMAKE_COMMAND) -P CMakeFiles/base_static.dir/cmake_clean_target.cmake
	cd /home/chenwen/Desktop/cpp/correct_det/src/base && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/base_static.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
base/CMakeFiles/base_static.dir/build: base/libbase_static.a

.PHONY : base/CMakeFiles/base_static.dir/build

base/CMakeFiles/base_static.dir/requires: base/CMakeFiles/base_static.dir/base_switches.cc.o.requires

.PHONY : base/CMakeFiles/base_static.dir/requires

base/CMakeFiles/base_static.dir/clean:
	cd /home/chenwen/Desktop/cpp/correct_det/src/base && $(CMAKE_COMMAND) -P CMakeFiles/base_static.dir/cmake_clean.cmake
.PHONY : base/CMakeFiles/base_static.dir/clean

base/CMakeFiles/base_static.dir/depend:
	cd /home/chenwen/Desktop/cpp/correct_det/src && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/chenwen/Desktop/cpp/correct_det/src /home/chenwen/Desktop/cpp/correct_det/src/base /home/chenwen/Desktop/cpp/correct_det/src /home/chenwen/Desktop/cpp/correct_det/src/base /home/chenwen/Desktop/cpp/correct_det/src/base/CMakeFiles/base_static.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : base/CMakeFiles/base_static.dir/depend

