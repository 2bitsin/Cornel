#!/bin/bash
mkdir -p build
mkdir -p bin
pushd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . --config Debug
cmake --install . --config Debug
popd

