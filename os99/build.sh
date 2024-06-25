
docker run -it                                           \
  -v $PROJECT_ROOT:/os99                                 \
  -e PROJECT_ROOT=/os99                                  \
  -e WORKSPACE_ROOT=/os99/workspace                      \
  os99-ia16                                              \
    /bin/bash -c 'cd /os99/os99/boot-pxe && ./build.sh'  