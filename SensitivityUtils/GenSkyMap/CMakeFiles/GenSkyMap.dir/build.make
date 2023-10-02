# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

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
CMAKE_COMMAND = /nevis/amsterdam/share/seligman/conda/grams/bin/cmake

# The command to remove a file.
RM = /nevis/amsterdam/share/seligman/conda/grams/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /nevis/milne/files/ms6556/Flat/GramsSimSensitivity

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /nevis/milne/files/ms6556/Flat/GramsSimSensitivity

# Include any dependencies generated for this target.
include SensitivityUtils/GenSkyMap/CMakeFiles/GenSkyMap.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include SensitivityUtils/GenSkyMap/CMakeFiles/GenSkyMap.dir/compiler_depend.make

# Include the progress variables for this target.
include SensitivityUtils/GenSkyMap/CMakeFiles/GenSkyMap.dir/progress.make

# Include the compile flags for this target's objects.
include SensitivityUtils/GenSkyMap/CMakeFiles/GenSkyMap.dir/flags.make

SensitivityUtils/GenSkyMap/CMakeFiles/GenSkyMap.dir/GenSkyMap.cpp.o: SensitivityUtils/GenSkyMap/CMakeFiles/GenSkyMap.dir/flags.make
SensitivityUtils/GenSkyMap/CMakeFiles/GenSkyMap.dir/GenSkyMap.cpp.o: SensitivityUtils/GenSkyMap/GenSkyMap.cpp
SensitivityUtils/GenSkyMap/CMakeFiles/GenSkyMap.dir/GenSkyMap.cpp.o: SensitivityUtils/GenSkyMap/CMakeFiles/GenSkyMap.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/nevis/milne/files/ms6556/Flat/GramsSimSensitivity/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object SensitivityUtils/GenSkyMap/CMakeFiles/GenSkyMap.dir/GenSkyMap.cpp.o"
	cd /nevis/milne/files/ms6556/Flat/GramsSimSensitivity/SensitivityUtils/GenSkyMap && /nevis/amsterdam/share/seligman/conda/grams/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT SensitivityUtils/GenSkyMap/CMakeFiles/GenSkyMap.dir/GenSkyMap.cpp.o -MF CMakeFiles/GenSkyMap.dir/GenSkyMap.cpp.o.d -o CMakeFiles/GenSkyMap.dir/GenSkyMap.cpp.o -c /nevis/milne/files/ms6556/Flat/GramsSimSensitivity/SensitivityUtils/GenSkyMap/GenSkyMap.cpp

SensitivityUtils/GenSkyMap/CMakeFiles/GenSkyMap.dir/GenSkyMap.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/GenSkyMap.dir/GenSkyMap.cpp.i"
	cd /nevis/milne/files/ms6556/Flat/GramsSimSensitivity/SensitivityUtils/GenSkyMap && /nevis/amsterdam/share/seligman/conda/grams/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /nevis/milne/files/ms6556/Flat/GramsSimSensitivity/SensitivityUtils/GenSkyMap/GenSkyMap.cpp > CMakeFiles/GenSkyMap.dir/GenSkyMap.cpp.i

SensitivityUtils/GenSkyMap/CMakeFiles/GenSkyMap.dir/GenSkyMap.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/GenSkyMap.dir/GenSkyMap.cpp.s"
	cd /nevis/milne/files/ms6556/Flat/GramsSimSensitivity/SensitivityUtils/GenSkyMap && /nevis/amsterdam/share/seligman/conda/grams/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /nevis/milne/files/ms6556/Flat/GramsSimSensitivity/SensitivityUtils/GenSkyMap/GenSkyMap.cpp -o CMakeFiles/GenSkyMap.dir/GenSkyMap.cpp.s

# Object files for target GenSkyMap
GenSkyMap_OBJECTS = \
"CMakeFiles/GenSkyMap.dir/GenSkyMap.cpp.o"

# External object files for target GenSkyMap
GenSkyMap_EXTERNAL_OBJECTS =

SensitivityUtils/GenSkyMap/GenSkyMap: SensitivityUtils/GenSkyMap/CMakeFiles/GenSkyMap.dir/GenSkyMap.cpp.o
SensitivityUtils/GenSkyMap/GenSkyMap: SensitivityUtils/GenSkyMap/CMakeFiles/GenSkyMap.dir/build.make
SensitivityUtils/GenSkyMap/GenSkyMap: SensitivityUtils/libGramsSensitivitySenseUtilities.a
SensitivityUtils/GenSkyMap/GenSkyMap: SensitivityUtils/libGramsSensitivityGenSkyMapUtils.a
SensitivityUtils/GenSkyMap/GenSkyMap: SensitivityUtils/libGramsSensitivityReadRoot.a
SensitivityUtils/GenSkyMap/GenSkyMap: /nevis/amsterdam/share/seligman/conda/grams/lib/libxerces-c.so
SensitivityUtils/GenSkyMap/GenSkyMap: SensitivityUtils/GenSkyMap/CMakeFiles/GenSkyMap.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/nevis/milne/files/ms6556/Flat/GramsSimSensitivity/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable GenSkyMap"
	cd /nevis/milne/files/ms6556/Flat/GramsSimSensitivity/SensitivityUtils/GenSkyMap && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/GenSkyMap.dir/link.txt --verbose=$(VERBOSE)
	cd /nevis/milne/files/ms6556/Flat/GramsSimSensitivity/SensitivityUtils/GenSkyMap && /nevis/amsterdam/share/seligman/conda/grams/bin/cmake -E copy /nevis/milne/files/ms6556/Flat/GramsSimSensitivity/SensitivityUtils/GenSkyMap/GenSkyMap /nevis/milne/files/ms6556/Flat/GramsSimSensitivity/Source/GramsWork
	cd /nevis/milne/files/ms6556/Flat/GramsSimSensitivity/SensitivityUtils/GenSkyMap && /nevis/amsterdam/share/seligman/conda/grams/bin/cmake -E copy /nevis/milne/files/ms6556/Flat/GramsSimSensitivity/SensitivityUtils/GenSkyMap/GenSkyMap /nevis/milne/files/ms6556/Flat/GramsSimSensitivity/Background/GramsWork
	cd /nevis/milne/files/ms6556/Flat/GramsSimSensitivity/SensitivityUtils/GenSkyMap && /nevis/amsterdam/share/seligman/conda/grams/bin/cmake -E copy /nevis/milne/files/ms6556/Flat/GramsSimSensitivity/SensitivityUtils/GenSkyMap/GenSkyMap /nevis/milne/files/ms6556/Flat/GramsSimSensitivity/EffArea/GramsWork

# Rule to build all files generated by this target.
SensitivityUtils/GenSkyMap/CMakeFiles/GenSkyMap.dir/build: SensitivityUtils/GenSkyMap/GenSkyMap
.PHONY : SensitivityUtils/GenSkyMap/CMakeFiles/GenSkyMap.dir/build

SensitivityUtils/GenSkyMap/CMakeFiles/GenSkyMap.dir/clean:
	cd /nevis/milne/files/ms6556/Flat/GramsSimSensitivity/SensitivityUtils/GenSkyMap && $(CMAKE_COMMAND) -P CMakeFiles/GenSkyMap.dir/cmake_clean.cmake
.PHONY : SensitivityUtils/GenSkyMap/CMakeFiles/GenSkyMap.dir/clean

SensitivityUtils/GenSkyMap/CMakeFiles/GenSkyMap.dir/depend:
	cd /nevis/milne/files/ms6556/Flat/GramsSimSensitivity && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /nevis/milne/files/ms6556/Flat/GramsSimSensitivity /nevis/milne/files/ms6556/Flat/GramsSimSensitivity/SensitivityUtils/GenSkyMap /nevis/milne/files/ms6556/Flat/GramsSimSensitivity /nevis/milne/files/ms6556/Flat/GramsSimSensitivity/SensitivityUtils/GenSkyMap /nevis/milne/files/ms6556/Flat/GramsSimSensitivity/SensitivityUtils/GenSkyMap/CMakeFiles/GenSkyMap.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : SensitivityUtils/GenSkyMap/CMakeFiles/GenSkyMap.dir/depend

