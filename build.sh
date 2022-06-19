#!/bin/bash

set -e

export BUILD_TYPE=Release
export BUILD_ROOT=$(pwd)/build

rm -rf workspace
mkdir -p $BUILD_ROOT/bootload
mkdir -p workspace/boot

pushd source/bootload.sys
wcc -3 -ms -ox -d0 bootload.c -fo=$BUILD_ROOT/bootload/bootload.o
wasm -3 -mt smallcode.asm -fo=$BUILD_ROOT/bootload/smallcode.o
wasm -3 -mt stacksegment.asm -fo=$BUILD_ROOT/bootload/stacksegment.o
popd 

pushd $BUILD_ROOT/bootload
wlink name bootload.sys format raw bin option offset=0x0000, nodefaultlibs, start=bootload_, map=bootload.sys.map file { bootload.o smallcode.o stacksegment.o } 
popd

cp $BUILD_ROOT/bootload/bootload.sys workspace/boot
mkisofs -input-charset utf8 -pad -R -J -c boot/catalogue -b boot/bootload.sys -no-emul-boot -boot-load-seg 0x7c0 -o $BUILD_ROOT/image.iso ./workspace