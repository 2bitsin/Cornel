#!/bin/bash
set -e

declare BUILD_ROOT=`pwd`


#pushd netboot16
#  rm -rf build
#  mkdir -p build
#  cd build
#
#  cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$WORKSPACE_ROOT -DCMAKE_TOOLCHAIN_FILE=$BUILD_ROOT/cmake/i86rm.cmake 
#  cmake --build . --config Release -- VERBOSE=1
#  cmake --install . --config Release
#
#  popd

pushd netboot32
  rm -rf build
  mkdir -p build
  cd build

  cmake .. -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_INSTALL_PREFIX=$WORKSPACE_ROOT -DCMAKE_TOOLCHAIN_FILE=$BUILD_ROOT/cmake/g++12.cmake 
  cmake --build . --config $BUILD_TYPE #-- VERBOSE=1
  cmake --install . --config $BUILD_TYPE

  popd


