cmake_minimum_required(VERSION 3.22.1)
project(libtextio LANGUAGES C CXX)

add_library(misc misc.cpp)
target_include_directories(misc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(misc PROPERTIES CXX_STANDARD 23)