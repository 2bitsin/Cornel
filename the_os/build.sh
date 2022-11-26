#!/bin/bash
set -e

declare BUILD_ROOT=`pwd`

mkdir -p build
pushd build

cmake .. -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_INSTALL_PREFIX=$WORKSPACE_ROOT -DCMAKE_TOOLCHAIN_FILE=$BUILD_ROOT/cmake/i686pm.cmake 
cmake --build . --config $BUILD_TYPE -- VERBOSE=1
cmake --install . --config $BUILD_TYPE

popd


