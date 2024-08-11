INSTALL_PATH=$WORKSPACE_ROOT/install
CONFIG_PATH=$WORKSPACE_ROOT/config.ini

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

docker run -it                                           \
  -v $PROJECT_ROOT:/os99                                 \
  -e PROJECT_ROOT=/os99                                  \
  -e BUILD_ROOT=/os99/$BUILD_ROOT                        \
  -e WORKSPACE_ROOT=/os99/$INSTALL_PATH                  \
  -e BUILD_TYPE=$BUILD_TYPE                              \
  -w /os99/os99/core                                     \
  os99-main                                              \
  bash -c -l ./build.sh

mkdir -p $PROJECT_ROOT/$INSTALL_PATH
cp ./config.ini $PROJECT_ROOT/$CONFIG_PATH

pushd $PROJECT_ROOT/$WORKSPACE_ROOT
  ./bin/bootwat ./bin/boot-pxe.exe ./install/boot-pxe.sys
popd

# $WORKSPACE_ROOT/boot-pxe.sys
