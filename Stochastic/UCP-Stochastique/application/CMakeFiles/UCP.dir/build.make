# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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
CMAKE_SOURCE_DIR = /home/jacquin/Bureau/UCP-Stochastique/application

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/jacquin/Bureau/UCP-Stochastique/application

# Include any dependencies generated for this target.
include CMakeFiles/UCP.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/UCP.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/UCP.dir/flags.make

CMakeFiles/UCP.dir/UCP.o: CMakeFiles/UCP.dir/flags.make
CMakeFiles/UCP.dir/UCP.o: UCP.cpp
	$(CMAKE_COMMAND) -E cmake_progress_report /home/jacquin/Bureau/UCP-Stochastique/application/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object CMakeFiles/UCP.dir/UCP.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/UCP.dir/UCP.o -c /home/jacquin/Bureau/UCP-Stochastique/application/UCP.cpp

CMakeFiles/UCP.dir/UCP.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/UCP.dir/UCP.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /home/jacquin/Bureau/UCP-Stochastique/application/UCP.cpp > CMakeFiles/UCP.dir/UCP.i

CMakeFiles/UCP.dir/UCP.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/UCP.dir/UCP.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /home/jacquin/Bureau/UCP-Stochastique/application/UCP.cpp -o CMakeFiles/UCP.dir/UCP.s

CMakeFiles/UCP.dir/UCP.o.requires:
.PHONY : CMakeFiles/UCP.dir/UCP.o.requires

CMakeFiles/UCP.dir/UCP.o.provides: CMakeFiles/UCP.dir/UCP.o.requires
	$(MAKE) -f CMakeFiles/UCP.dir/build.make CMakeFiles/UCP.dir/UCP.o.provides.build
.PHONY : CMakeFiles/UCP.dir/UCP.o.provides

CMakeFiles/UCP.dir/UCP.o.provides.build: CMakeFiles/UCP.dir/UCP.o

# Object files for target UCP
UCP_OBJECTS = \
"CMakeFiles/UCP.dir/UCP.o"

# External object files for target UCP
UCP_EXTERNAL_OBJECTS =

UCP: CMakeFiles/UCP.dir/UCP.o
UCP: CMakeFiles/UCP.dir/build.make
UCP: CMakeFiles/UCP.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX executable UCP"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/UCP.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/UCP.dir/build: UCP
.PHONY : CMakeFiles/UCP.dir/build

CMakeFiles/UCP.dir/requires: CMakeFiles/UCP.dir/UCP.o.requires
.PHONY : CMakeFiles/UCP.dir/requires

CMakeFiles/UCP.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/UCP.dir/cmake_clean.cmake
.PHONY : CMakeFiles/UCP.dir/clean

CMakeFiles/UCP.dir/depend:
	cd /home/jacquin/Bureau/UCP-Stochastique/application && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/jacquin/Bureau/UCP-Stochastique/application /home/jacquin/Bureau/UCP-Stochastique/application /home/jacquin/Bureau/UCP-Stochastique/application /home/jacquin/Bureau/UCP-Stochastique/application /home/jacquin/Bureau/UCP-Stochastique/application/CMakeFiles/UCP.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/UCP.dir/depend

