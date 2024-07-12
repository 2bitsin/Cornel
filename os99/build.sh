echo "WORKSPACE_ROOT=" $WORKSPACE_ROOT
docker run -it                                           \
  -v $PROJECT_ROOT:/os99                                 \
  -e PROJECT_ROOT=/os99                                  \
  -e BUILD_ROOT=/os99/$BUILD_ROOT                        \
  -e WORKSPACE_ROOT=/os99/$WORKSPACE_ROOT                \
  -e BUILD_TYPE=$BUILD_TYPE                              \
  -w /os99/os99/boot-pxe                                 \
  os99-ia16                                              \
  bash -c -l ./build.sh