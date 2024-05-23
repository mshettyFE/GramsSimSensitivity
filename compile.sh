#!/bin/bash

# Build project
WorkingDirName=build
ProjectName=GramsSimSensitivity
mkdir ../$WorkingDirName
cd ../$WorkingDirName
cmake ../$ProjectName
make

# Move over relevant files from source to build
mkdir EffArea
cp -r ../$ProjectName/GramsWork EffArea/
cp ../$ProjectName/ConfigFiles/* EffArea/
cp ../$ProjectName/GenCondorJobs.py EffArea/
cp ../$ProjectName/CalcEffArea.py EffArea/

mkdir Sensitivity
cp ../$ProjectName/ConfigFiles/* Sensitivity/
cp ../$ProjectName/GenCondorJobs.py Sensitivity/
cp ../$ProjectName/CalculateSensitivity.py Sensitivity/

mkdir Source
cp -r ../$ProjectName/GramsWork Source/
cp ../$ProjectName/ConfigFiles/* Source/
cp ../$ProjectName/GenCondorJobs.py Source/
cp ../$ProjectName/GramsWork/options.xml Source/
cp ../$ProjectName/GenMask.py Source/

mkdir Background
cp -r ../$ProjectName/GramsWork Background/
cp ../$ProjectName/ConfigFiles/* Background/
cp ../$ProjectName/GenCondorJobs.py Background/
cp ../$ProjectName/GramsWork/options.xml Background/

# Move condor stray file cleaner to build dir
cp ../$ProjectName/RemoveClutter.sh .

# Clean up CMake stuff
rmdir root
rm -rf CMakeFiles
rm -f cmake_install.cmake
rm CMakeCache.txt
rm Makefile