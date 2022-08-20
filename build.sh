#!/bin/bash

export WORKSPACE_ROOT=`pwd`/workspace

rm -rf $WORKSPACE_ROOT
mkdir -p $WORKSPACE_ROOT

pushd the_os  
  
  source ./build.sh

  popd