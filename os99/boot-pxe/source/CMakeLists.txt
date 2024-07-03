cmake_minimum_required(VERSION 3.22.0)
project(boot-pxe)
add_executable(boot-pxe main.c)
install(TARGETS boot-pxe DESTINATION ${CMAKE_INSTALL_PREFIX})
