export IMAGE_PATH=$WORKSPACE_ROOT/os99core.img
export IMAGE_SIZE=16M

cmake -B ${BUILD_ROOT}/os99core -S .              \
  -DCMAKE_VERBOSE_MAKEFILE=YES                    \
  -DCMAKE_INSTALL_PREFIX=$WORKSPACE_ROOT          \
  -DCMAKE_BUILD_TYPE=$BUILD_TYPE                  
cmake --build ${BUILD_ROOT}/os99core --config $BUILD_TYPE
cmake --install ${BUILD_ROOT}/os99core --config $BUILD_TYPE

dd if=/dev/zero of=$IMAGE_PATH bs=$IMAGE_SIZE count=1
mkfs.vfat -F 16 $IMAGE_PATH
mcopy -i $IMAGE_PATH -s $WORKSPACE_ROOT/image/* ::/
