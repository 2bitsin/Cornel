cmake -B ${BUILD_ROOT}/os99core -S .              \
  -DCMAKE_VERBOSE_MAKEFILE=YES                    \
  -DCMAKE_INSTALL_PREFIX=$WORKSPACE_ROOT          \
  -DCMAKE_BUILD_TYPE=$BUILD_TYPE                  
cmake --build ${BUILD_ROOT}/os99core --config $BUILD_TYPE
cmake --install ${BUILD_ROOT}/os99core --config $BUILD_TYPE