# Adapted from https://github.com/wgseligman/GramsSim

# Specify minimum cmake version and terminate if not fulfilled
cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

# Set C++ Standard to C++17 to enable use of filesystem
set (CMAKE_CXX_STANDARD 17)

project(GramsSensitivity)

# Check for Mac OS X Darwin
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set(MACOSX TRUE)
endif()

# If we have Mac OS, we need to add .exe to all the executables

set(EXE "")
if (MACOSX)
   set (EXE ".exe")
   add_definitions(-DEXE_SUFFIX=.exe)
endif()

# Add this project's cmake/Module directory as a source of our own CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/Modules)

# Include any special macros
include(macros)

# Set up ROOT
include(cmake/ROOT.cmake)

# Find Xerces-C XML parser for utils
find_package (XercesC REQUIRED)

# Find Eigen library for GenSkyMapUtils
find_package(Eigen3 REQUIRED)
link_directories(${EIGEN_INCLUDE_DIRS})

# Set the rpath so that the executables can find the libraries
# even if the environment setups have not been run in the shell (ie. even if module load hasn't been run)
set(CMAKE_INSTALL_RPATH "${ROOT_LIBRARY_DIR}")

# Copy over Background
file(GLOB BACKGROUND_RECON_FILES "${CMAKE_SOURCE_DIR}/Background/*")
file( COPY ${BACKGROUND_RECON_FILES} DESTINATION ${CMAKE_BINARY_DIR}/Background)

# Copy over Source
file(GLOB SOURCE_RECON_FILES "${CMAKE_SOURCE_DIR}/Source/*")
file( COPY ${SOURCE_RECON_FILES} DESTINATION ${CMAKE_BINARY_DIR}/Source)

# Make Sensitivity Directory
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/Sensitivity)

# Copy over ConfigFiles to Background,EffArea, Source
file(GLOB CONFIG_FILES "${CMAKE_SOURCE_DIR}/ConfigFiles/*")
file( COPY ${CONFIG_FILES} DESTINATION ${CMAKE_BINARY_DIR}/Background)
file( COPY ${CONFIG_FILES} DESTINATION ${CMAKE_BINARY_DIR}/EffArea)
file( COPY ${CONFIG_FILES} DESTINATION ${CMAKE_BINARY_DIR}/Source)
file( COPY ${CONFIG_FILES} DESTINATION ${CMAKE_BINARY_DIR}/Sensitivity)

# Copy over GenCondorJobs
file(GLOB GENERATION_SCRIPT "${CMAKE_SOURCE_DIR}/SensitivityUtils/GenCondorJobs.py")
file( COPY ${GENERATION_SCRIPT} DESTINATION ${CMAKE_BINARY_DIR}/Background)
file( COPY ${GENERATION_SCRIPT} DESTINATION ${CMAKE_BINARY_DIR}/EffArea)
file( COPY ${GENERATION_SCRIPT} DESTINATION ${CMAKE_BINARY_DIR}/Source)
file( COPY ${GENERATION_SCRIPT} DESTINATION ${CMAKE_BINARY_DIR}/Sensitivity)

# Copy over GenCondorJobs to Background, EffArea, Source, Sensitivity

#Copy over options.xml to Source
file( COPY ${CMAKE_SOURCE_DIR}/GramsWork/options.xml DESTINATION ${CMAKE_BINARY_DIR}/Source)

# Copy over RemoveClutter.sh

file( COPY ${CMAKE_SOURCE_DIR}/RemoveClutter.sh DESTINATION ${CMAKE_BINARY_DIR})

add_subdirectory(SensitivityUtils)
add_subdirectory(EffArea)
add_subdirectory(Source)

# Copy over GramsWork containing compiled GramsSim programs to  Background and Source
file( COPY ${CMAKE_SOURCE_DIR}/GramsWork DESTINATION ${CMAKE_BINARY_DIR}/Background)
file( COPY ${CMAKE_SOURCE_DIR}/GramsWork DESTINATION ${CMAKE_BINARY_DIR}/Source)
file( COPY ${CMAKE_SOURCE_DIR}/GramsWork DESTINATION ${CMAKE_BINARY_DIR}/EffArea)

# Copy over GenCondorJobs to EffArea, Background, and Source
file( COPY ${CMAKE_SOURCE_DIR}/SensitivityUtils/GenCondorJobs.py DESTINATION ${CMAKE_BINARY_DIR}/Background)
file( COPY ${CMAKE_SOURCE_DIR}/SensitivityUtils/GenCondorJobs.py DESTINATION ${CMAKE_BINARY_DIR}/EffArea)
file( COPY ${CMAKE_SOURCE_DIR}/SensitivityUtils/GenCondorJobs.py DESTINATION ${CMAKE_BINARY_DIR}/Source)