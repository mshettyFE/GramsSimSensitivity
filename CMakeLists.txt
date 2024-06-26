# Adapted from https://github.com/wgseligman/GramsSim

# Specify minimum cmake version and terminate if not fulfilled
cmake_minimum_required(VERSION 3.6 FATAL_ERROR)

# Set C++ Standard to C++17 to enable use of filesystem
set (CMAKE_CXX_STANDARD 17)

# Turn Warnings on for all targets
if (MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic)
endif()

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

add_subdirectory(SensitivityUtils)