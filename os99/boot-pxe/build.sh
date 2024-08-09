#!/bin/bash -l

set -e

BUILD_PATH=$BUILD_ROOT/boot-pxe
mkdir -p $BUILD_ROOT/boot-pxe

cmake -B $BUILD_PATH                      \
  -G "Watcom WMake"                       \
  -D CMAKE_VERBOSE_MAKEFILE=YES           \
  -D CMAKE_INSTALL_PREFIX=$WORKSPACE_ROOT \
  -D CMAKE_BUILD_TYPE=$BUILD_TYPE         \
  -D CMAKE_SYSTEM_NAME=DOS                \
  -D CMAKE_SYSTEM_PROCESSOR=I86           \
  -S .    
cmake --build $BUILD_PATH --config $BUILD_TYPE
cmake --install $BUILD_PATH --config $BUILD_TYPE


