#!/bin/bash
git submodule init
mkdir GramsWork
cd GramsWork
if [ $? -ne 0 ]
then
  exit
fi
cmake ../GramsSim
make
rm -rf GramsDetSim
rm -rf GramsElecSim
rm -rf GramsG4
rm -rf GramsReadoutSim
rm -rf GramsSky
rm -rf root
rm -rf scripts
rm -rf bin
rm -rf CMakeFiles
rm -f CMakeCache.txt cmake_install.cmake Makefile
cp ./gdml/* .
cp ../ConfigFiles/SensitivityOptions.xml .
