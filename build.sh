#!/bin/bash

set -e

export BUILD_TYPE=Release
export BUILD_ROOT=$(pwd)/build

pushd ./source/coloader
BUILD_ROOT=$BUILD_ROOT/coloader ./build.sh
popd

cp $BUILD_ROOT/coloader/coloader.sys $BUILD_ROOT/../workspace/boot/coloader.sys
truncate -s 2K $BUILD_ROOT/../workspace/boot/coloader.sys

mkisofs -input-charset utf8 -pad -R -J -c boot/catalogue -b boot/coloader.sys -no-emul-boot -boot-load-seg 0x800 -o $BUILD_ROOT/cornel.iso $BUILD_ROOT/../workspace