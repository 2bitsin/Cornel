#!/bin/bash

set -e

export BUILD_TYPE=Release
export BUILD_ROOT=$(pwd)/build

rm -rf workspace
rm -rf source/bootload.sys/*.err
mkdir -p $BUILD_ROOT/bootload
mkdir -p workspace/boot

pushd source/bootload.sys

declare LOAD_SEGMENT="0x7c0"
declare -a SOURCES=("bootstrap.asm" "memquery.asm" "smallcode.asm" "stackseg.asm" "longdiv.asm" "bootstrap.c" "print.c" "error.c" "memmap.c")
declare -a OBJECTS=()
declare C_FLAGS="-3 -ms -ox -zu -dDEBUG=1 -dG_LOAD_SEGMENT=$LOAD_SEGMENT"
declare S_FLAGS="-3 -mt -dG_LOAD_SEGMENT=$LOAD_SEGMENT"

for src in ${SOURCES[@]} ; do
  if [[ "$src" == *.c ]]
  then
    echo "Compiling $src ..."
    echo "wcc -i=./include $C_FLAGS $src -fo=$BUILD_ROOT/bootload/$src.o"
    wcc -i=./include $C_FLAGS $src -fo=$BUILD_ROOT/bootload/$src.o
    OBJECTS+=("$src.o")
  else 
    echo "Compiling $src ..."
    echo "wasm $S_FLAGS ./asm/$src -fo=$BUILD_ROOT/bootload/$src.o"
    wasm $S_FLAGS ./asm/$src -fo=$BUILD_ROOT/bootload/$src.o
    OBJECTS+=("$src.o")
  fi
done

popd 

echo "{ ${OBJECTS[@]} }"

pushd $BUILD_ROOT/bootload
wlink \
  name bootload.sys \
  format raw bin \
  option \
    offset=0x0000, \
    nodefaultlibs, \
    start=_bootstrap, \
    map=bootload.sys.map \
  file { ${OBJECTS[@]} }
popd

cp $BUILD_ROOT/bootload/bootload.sys workspace/boot
mkisofs -input-charset utf8 -pad -R -J -c boot/catalogue -b boot/bootload.sys -no-emul-boot -boot-load-seg 0x7c0 -o $BUILD_ROOT/image.iso ./workspace