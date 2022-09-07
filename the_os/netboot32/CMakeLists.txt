cmake_minimum_required(VERSION 3.22)
project(netboot32 C CXX ASM)

add_subdirectory(../libcimpl ${CMAKE_BINARY_DIR}/libcimpl)
add_subdirectory(../libhardware ${CMAKE_BINARY_DIR}/libhardware)
add_subdirectory(../libtextio ${CMAKE_BINARY_DIR}/libtextio)
add_subdirectory(../libmisc ${CMAKE_BINARY_DIR}/libmisc)
add_subdirectory(source)

install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/config.ini DESTINATION ${CMAKE_INSTALL_PREFIX})
