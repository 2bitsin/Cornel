cmake_minimum_required(VERSION 3.22.0)
project(the_os VERSION 0.1.0 LANGUAGES C CXX ASM)

add_subdirectory(libcimpl)
add_subdirectory(libhardware)
add_subdirectory(libtextio)
add_subdirectory(libmemory)
add_subdirectory(libutils)
add_subdirectory(config)
add_subdirectory(netboot32)

