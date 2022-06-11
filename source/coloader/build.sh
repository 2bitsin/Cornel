#!/bin/bash

set -e

CC_FLAGS="-3 -oh -ol -ol+ -os -om -or -t=2 -ecc -zl -s -ms -bt=dos"
CXX_FLAGS="$CC_FLAGS -xd"
ASM_FLAGS="-3 -mt"

function cc() {
  for arg do
    wcc $CC_FLAGS $arg -fo=$BUILD_ROOT/$arg.o -fr=$BUILD_ROOT/$arg.err
    printf "\n\n"
  done
}

function cxx() {
  for arg do
    wpp $CXX_FLAGS $arg -fo=$BUILD_ROOT/$arg.o -fr=$BUILD_ROOT/$arg.err
    printf "\n\n"
  done
}

function asm() {
  for arg do 
    printf "%s\n\n" "wasm $ASM_FLAGS $arg -fo=$BUILD_ROOT/$arg.o -fr=$BUILD_ROOT/$arg.err"
    wasm $ASM_FLAGS $arg -fo=$BUILD_ROOT/$arg.o -fr=$BUILD_ROOT/$arg.err
    printf "\n\n"
  done
}

function ld() {  
  wlink                             \
    name $BUILD_ROOT/coloader.sys   \
    format raw bin                  \
    option                          \
      offset=0x8000,                \
      start=_coloader_init,         \
      map=$BUILD_ROOT/coloader.map  \
    path $BUILD_ROOT                \
    file \{ "$@" \}
}

rm    -rf $BUILD_ROOT/*

asm   init.asm
cxx   main.cpp bios.cpp linea20.cpp memory.cpp
ld    init.asm.o      \
      bios.cpp.o      \
      main.cpp.o      \
      memory.cpp.o    \
      linea20.cpp.o
