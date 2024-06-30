cmake_minimum_required(VERSION 3.22.0)
project(boot-pxe)
add_executable(boot-pxe 
  main.c)
#target_link_options(boot-pxe PUBLIC LINKER:MAP=${CMAKE_BINARY_DIR}/boot-pxe.map)  
install(TARGETS boot-pxe DESTINATION ${CMAKE_INSTALL_PREFIX})
