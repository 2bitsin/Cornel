#!/bin/bash

set -e

export BUILD_TYPE=Release
export BUILD_ROOT=$(pwd)/build

fasm source/muloader/stub.asm $BUILD_ROOT/stub.bin
wpp -3 -ms -dNDEBUG=1 -d0 -ox -i=$WATCOM/h source/muloader/main.cpp -fo=$BUILD_ROOT/main.o
wlink name build/muloader.com format dos com libpath $WATCOM/lib286:$WATCOM/lib286/dos file \{ $BUILD_ROOT/main.o \}
cat $BUILD_ROOT/stub.bin build/muloader.com >workspace/boot/coloader.sys

mkisofs -input-charset utf8 -pad -R -J -c boot/catalogue -b boot/coloader.sys -no-emul-boot -boot-load-seg 0x7c0 -o $BUILD_ROOT/cornel.iso ./workspace