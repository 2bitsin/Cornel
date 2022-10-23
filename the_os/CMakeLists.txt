cmake_minimum_required(VERSION 3.22.0)
project(the_os VERSION 0.1.0 LANGUAGES C CXX ASM)

#add_definitions(-DNO_DEBUG_LOGS)

add_subdirectory(library/cimpl)
add_subdirectory(library/hardware)
add_subdirectory(library/textio)
add_subdirectory(library/memory)
add_subdirectory(library/utils)
add_subdirectory(config)
add_subdirectory(netboot32)

