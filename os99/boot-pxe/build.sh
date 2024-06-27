#!/bin/bash -l


# local variables
TEMPORARY_PATH=$WORKSPACE_ROOT/build/boot-pxe
PREABLE_PATH=$TEMPORARY_PATH/preamble.bin
TARGET_PATH=$WORKSPACE_ROOT/boot-pxe.sys
CONFIG_PATH=$WORKSPACE_ROOT/config.ini
EXECUTABLE_PATH=$TEMPORARY_PATH/boot-pxe/boot-pxe.exe

mkdir -p $TEMPORARY_PATH

fasm bootstrap/main.asm $PREABLE_PATH

cmake -B $TEMPORARY_PATH/boot-pxe         \
  -G "Watcom WMake"                       \
  -D CMAKE_INSTALL_PREFIX=$TEMPORARY_PATH \
  -D CMAKE_BUILD_TYPE=$BUILD_TYPE         \
  -D CMAKE_SYSTEM_NAME=DOS                \
  -D CMAKE_SYSTEM_PROCESSOR=I86           \
  -S source                  
cmake --build   $TEMPORARY_PATH/boot-pxe --config $BUILD_TYPE
cmake --install $TEMPORARY_PATH/boot-pxe 

cat $PREABLE_PATH $EXECUTABLE_PATH >$TARGET_PATH
cp ./config.ini $CONFIG_PATH