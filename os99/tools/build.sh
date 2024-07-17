#!/bin/bash
mkdir -p build
mkdir -p bin
pushd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release
cmake --install . --config Release
popd

