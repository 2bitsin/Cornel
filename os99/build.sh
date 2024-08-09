CONFIG_PATH=$WORKSPACE_ROOT/../config.ini

pushd bootwat
  ./build.sh
popd

docker run -it                                           \
  -v $PROJECT_ROOT:/os99                                 \
  -e PROJECT_ROOT=/os99                                  \
  -e BUILD_ROOT=/os99/$BUILD_ROOT                        \
  -e WORKSPACE_ROOT=/os99/$WORKSPACE_ROOT                \
  -e BUILD_TYPE=$BUILD_TYPE                              \
  -w /os99/os99/boot-pxe                                 \
  os99-ia16                                              \
  bash -c -l ./build.sh

#docker run -it                                           \
#  -v $PROJECT_ROOT:/os99                                 \
#  -e PROJECT_ROOT=/os99                                  \
#  -e BUILD_ROOT=/os99/$BUILD_ROOT                        \
#  -e WORKSPACE_ROOT=/os99/$WORKSPACE_ROOT                \
#  -e BUILD_TYPE=$BUILD_TYPE                              \
#  -w /os99/os99/core                                     \
#  os99-main                                              \
#  bash -c -l ./build.sh
#
#cp ./config.ini $CONFIG_PATH
#
#$WORKSPACE_ROOT/bin/bootwat $WORKSPACE_ROOT/


# $WORKSPACE_ROOT/boot-pxe.sys
