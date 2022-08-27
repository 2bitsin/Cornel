cmake_minimum_required(VERSION 3.22)
project(netboot32 C CXX ASM)
add_subdirectory(source)

install(TARGETS netboot32 DESTINATION ${CMAKE_INSTALL_PREFIX})
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/config.ini DESTINATION ${CMAKE_INSTALL_PREFIX})
