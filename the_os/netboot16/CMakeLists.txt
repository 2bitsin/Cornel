cmake_minimum_required(VERSION 3.12)
project(netboot C CXX ASM)
add_subdirectory(source)
install(TARGETS netboot16 DESTINATION ${CMAKE_INSTALL_PREFIX})