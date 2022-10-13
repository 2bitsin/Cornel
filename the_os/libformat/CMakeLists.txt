cmake_minimum_required (VERSION 3.18)
project (format)

add_executable (format main.cpp)

set_target_properties (format PROPERTIES CXX_STANDARD 23)