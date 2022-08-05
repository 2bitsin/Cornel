cmake_minimum_required(VERSION 3.18)
project(common_library)
add_library(common STATIC arguments.cpp crc32.cpp "conf_ini.cpp")
set_property(TARGET common PROPERTY CXX_STANDARD 23)