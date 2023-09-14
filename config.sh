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
