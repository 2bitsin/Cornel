#!/bin/sh

pushd ./netboot32
  cmake --build ./build --target clean
  popd
