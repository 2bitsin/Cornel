#!/bin/bash

export BUILD_TYPE=Release
#export BUILD_TYPE=Debug

pushd ./os99
  source ./clean.sh
  popd
  
