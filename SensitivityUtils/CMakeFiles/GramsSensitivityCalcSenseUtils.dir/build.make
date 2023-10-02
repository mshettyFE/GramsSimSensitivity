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
include SensitivityUtils/CMakeFiles/GramsSensitivityCalcSenseUtils.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include SensitivityUtils/CMakeFiles/GramsSensitivityCalcSenseUtils.dir/compiler_depend.make

# Include the progress variables for this target.
include SensitivityUtils/CMakeFiles/GramsSensitivityCalcSenseUtils.dir/progress.make

# Include the compile flags for this target's objects.
include SensitivityUtils/CMakeFiles/GramsSensitivityCalcSenseUtils.dir/flags.make

SensitivityUtils/CMakeFiles/GramsSensitivityCalcSenseUtils.dir/src/CalcSenseUtils.cpp.o: SensitivityUtils/CMakeFiles/GramsSensitivityCalcSenseUtils.dir/flags.make
SensitivityUtils/CMakeFiles/GramsSensitivityCalcSenseUtils.dir/src/CalcSenseUtils.cpp.o: SensitivityUtils/src/CalcSenseUtils.cpp
SensitivityUtils/CMakeFiles/GramsSensitivityCalcSenseUtils.dir/src/CalcSenseUtils.cpp.o: SensitivityUtils/CMakeFiles/GramsSensitivityCalcSenseUtils.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --progress-dir=/nevis/milne/files/ms6556/Flat/GramsSimSensitivity/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object SensitivityUtils/CMakeFiles/GramsSensitivityCalcSenseUtils.dir/src/CalcSenseUtils.cpp.o"
	cd /nevis/milne/files/ms6556/Flat/GramsSimSensitivity/SensitivityUtils && /nevis/amsterdam/share/seligman/conda/grams/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT SensitivityUtils/CMakeFiles/GramsSensitivityCalcSenseUtils.dir/src/CalcSenseUtils.cpp.o -MF CMakeFiles/GramsSensitivityCalcSenseUtils.dir/src/CalcSenseUtils.cpp.o.d -o CMakeFiles/GramsSensitivityCalcSenseUtils.dir/src/CalcSenseUtils.cpp.o -c /nevis/milne/files/ms6556/Flat/GramsSimSensitivity/SensitivityUtils/src/CalcSenseUtils.cpp

SensitivityUtils/CMakeFiles/GramsSensitivityCalcSenseUtils.dir/src/CalcSenseUtils.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Preprocessing CXX source to CMakeFiles/GramsSensitivityCalcSenseUtils.dir/src/CalcSenseUtils.cpp.i"
	cd /nevis/milne/files/ms6556/Flat/GramsSimSensitivity/SensitivityUtils && /nevis/amsterdam/share/seligman/conda/grams/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /nevis/milne/files/ms6556/Flat/GramsSimSensitivity/SensitivityUtils/src/CalcSenseUtils.cpp > CMakeFiles/GramsSensitivityCalcSenseUtils.dir/src/CalcSenseUtils.cpp.i

SensitivityUtils/CMakeFiles/GramsSensitivityCalcSenseUtils.dir/src/CalcSenseUtils.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green "Compiling CXX source to assembly CMakeFiles/GramsSensitivityCalcSenseUtils.dir/src/CalcSenseUtils.cpp.s"
	cd /nevis/milne/files/ms6556/Flat/GramsSimSensitivity/SensitivityUtils && /nevis/amsterdam/share/seligman/conda/grams/bin/x86_64-conda-linux-gnu-c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /nevis/milne/files/ms6556/Flat/GramsSimSensitivity/SensitivityUtils/src/CalcSenseUtils.cpp -o CMakeFiles/GramsSensitivityCalcSenseUtils.dir/src/CalcSenseUtils.cpp.s

# Object files for target GramsSensitivityCalcSenseUtils
GramsSensitivityCalcSenseUtils_OBJECTS = \
"CMakeFiles/GramsSensitivityCalcSenseUtils.dir/src/CalcSenseUtils.cpp.o"

# External object files for target GramsSensitivityCalcSenseUtils
GramsSensitivityCalcSenseUtils_EXTERNAL_OBJECTS =

SensitivityUtils/libGramsSensitivityCalcSenseUtils.a: SensitivityUtils/CMakeFiles/GramsSensitivityCalcSenseUtils.dir/src/CalcSenseUtils.cpp.o
SensitivityUtils/libGramsSensitivityCalcSenseUtils.a: SensitivityUtils/CMakeFiles/GramsSensitivityCalcSenseUtils.dir/build.make
SensitivityUtils/libGramsSensitivityCalcSenseUtils.a: SensitivityUtils/CMakeFiles/GramsSensitivityCalcSenseUtils.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --green --bold --progress-dir=/nevis/milne/files/ms6556/Flat/GramsSimSensitivity/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libGramsSensitivityCalcSenseUtils.a"
	cd /nevis/milne/files/ms6556/Flat/GramsSimSensitivity/SensitivityUtils && $(CMAKE_COMMAND) -P CMakeFiles/GramsSensitivityCalcSenseUtils.dir/cmake_clean_target.cmake
	cd /nevis/milne/files/ms6556/Flat/GramsSimSensitivity/SensitivityUtils && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/GramsSensitivityCalcSenseUtils.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
SensitivityUtils/CMakeFiles/GramsSensitivityCalcSenseUtils.dir/build: SensitivityUtils/libGramsSensitivityCalcSenseUtils.a
.PHONY : SensitivityUtils/CMakeFiles/GramsSensitivityCalcSenseUtils.dir/build

SensitivityUtils/CMakeFiles/GramsSensitivityCalcSenseUtils.dir/clean:
	cd /nevis/milne/files/ms6556/Flat/GramsSimSensitivity/SensitivityUtils && $(CMAKE_COMMAND) -P CMakeFiles/GramsSensitivityCalcSenseUtils.dir/cmake_clean.cmake
.PHONY : SensitivityUtils/CMakeFiles/GramsSensitivityCalcSenseUtils.dir/clean

SensitivityUtils/CMakeFiles/GramsSensitivityCalcSenseUtils.dir/depend:
	cd /nevis/milne/files/ms6556/Flat/GramsSimSensitivity && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /nevis/milne/files/ms6556/Flat/GramsSimSensitivity /nevis/milne/files/ms6556/Flat/GramsSimSensitivity/SensitivityUtils /nevis/milne/files/ms6556/Flat/GramsSimSensitivity /nevis/milne/files/ms6556/Flat/GramsSimSensitivity/SensitivityUtils /nevis/milne/files/ms6556/Flat/GramsSimSensitivity/SensitivityUtils/CMakeFiles/GramsSensitivityCalcSenseUtils.dir/DependInfo.cmake "--color=$(COLOR)"
.PHONY : SensitivityUtils/CMakeFiles/GramsSensitivityCalcSenseUtils.dir/depend

