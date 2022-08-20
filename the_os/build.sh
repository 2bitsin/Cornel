#!/bin/bash
set -e

declare BUILD_ROOT=`pwd`
pushd netboot
  rm -rf build
  mkdir -p build
  cd build

  cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=$WORKSPACE_ROOT -DCMAKE_TOOLCHAIN_FILE=$BUILD_ROOT/cmake/i86rm.cmake 
  cmake --build . --config Release -- VERBOSE=1
  cmake --install . --config Release

  popd

