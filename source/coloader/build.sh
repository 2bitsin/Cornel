#!/bin/bash

set -e

CC_FLAGS="-3 -oh -ol -ol+ -os -om -or -t=2 -zl -s -ms -bt=dos"
CXX_FLAGS="$CC_FLAGS -xd"
ASM_FLAGS="-3 -mt"

function cc() {
  for arg do
    printf "\nProcessing \033[31;1;4m%s\033[0m\n\n" $arg
    wcc $CC_FLAGS $arg -fo=$BUILD_ROOT/$arg.o -fr=$BUILD_ROOT/$arg.err
    printf "\n"
  done
}

function cxx() {
  for arg do
    printf "\nProcessing \033[31;1;4m%s\033[0m\n\n" $arg
    wpp $CXX_FLAGS $arg -fo=$BUILD_ROOT/$arg.o -fr=$BUILD_ROOT/$arg.err
    printf "\n"
  done
}

function asm() {
  for arg do 
    printf "\nProcessing \033[31;1;4m%s\033[0m\n\n" $arg
    wasm $ASM_FLAGS $arg -fo=$BUILD_ROOT/$arg.o -fr=$BUILD_ROOT/$arg.err
    printf "\n"
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

asm   startup.asm int32.asm
cxx   main.cpp bios.cpp linea20.cpp memory.cpp termio.cpp charconv.cpp
ld    startup.asm.o   \
      int32.asm.o     \
      bios.cpp.o      \
      main.cpp.o      \
      memory.cpp.o    \
      linea20.cpp.o   \
      termio.cpp.o    \
      charconv.cpp.o  
