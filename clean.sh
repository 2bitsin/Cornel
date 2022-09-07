#!/bin/bash

export BUILD_TYPE=Release
#export BUILD_TYPE=Debug

pushd ./the_os
  source ./clean.sh
  popd
  
