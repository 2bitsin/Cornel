#!/bin/bash

set -e

export BUILD_TYPE=Release
export BUILD_ROOT=$(pwd)/build

rm -rf workspace
rm -rf source/bootload/*.err
mkdir -p $BUILD_ROOT/bootload
mkdir -p workspace/boot

pushd source/bootload

declare LOAD_ADDRESS="0x8000"
declare LOAD_SEGMENT="0x800"
declare STACK_SIZE="0x2000"


declare -a SOURCES=("stackseg.asm" "bootstrap.asm" "smallcode.asm" "longdiv.asm" "memory.asm" "memory.c" "bootstrap.c" "print.c" "error.c")
declare -a OBJECTS=()
declare DEFINES="-dDEBUG=1 -dG_LOAD_ADDRESS=$LOAD_ADDRESS -dG_STACK_SIZE=$STACK_SIZE"
declare C_FLAGS="-3 -ms -ox -zu $DEFINES"
declare S_FLAGS="-3 -mt $DEFINES"

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
wlink                     \
  name bootload.sys       \
  format raw bin          \
  option                  \
    dosseg,               \
    nodefaultlibs,        \
    start=_bootstrap,     \
    map=bootload.sys.map, \
    offset=$STACK_SIZE,   \
    stack=0               \
  file { ${OBJECTS[@]} }
popd

cp $BUILD_ROOT/bootload/bootload.sys workspace/boot
mkisofs -input-charset utf8 -pad -R -J -c boot/catalogue -b boot/bootload.sys -no-emul-boot -boot-load-seg $LOAD_SEGMENT -o $BUILD_ROOT/image.iso ./workspace