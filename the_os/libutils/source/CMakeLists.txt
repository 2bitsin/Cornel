cmake_minimum_required(VERSION 3.22.1)
project(libutils LANGUAGES C CXX)

add_library(utils utils.cpp)
target_include_directories(utils PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(utils PROPERTIES CXX_STANDARD 23)