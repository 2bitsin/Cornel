#!/bin/bash

export PROJECT_ROOT=`pwd`
export BUILD_ROOT=build
export WORKSPACE_ROOT=workspace
export BUILD_TYPE=Release
#export BUILD_TYPE=Debug

rm -rf $WORKSPACE_ROOT
mkdir -p $WORKSPACE_ROOT

# Build the os99 docker images
pushd ./docker
  source ./build.sh
  popd

pushd ./os99
  source ./build.sh
  popd
  