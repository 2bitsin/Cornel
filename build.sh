#!/bin/bash

export WORKSPACE_ROOT=`pwd`/workspace
export BUILD_TYPE=Release
#export BUILD_TYPE=Debug

rm -rf $WORKSPACE_ROOT
mkdir -p $WORKSPACE_ROOT

pushd ./the_os
  source ./build.sh
  popd
  