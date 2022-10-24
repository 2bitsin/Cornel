cmake_minimum_required(VERSION 3.22.1)
project(libscript LANGUAGES C CXX)

set (LIB_NAME script)
add_subdirectory(source)

if(NOT ${CMAKE_SYSTEM_NAME} MATCHES "Generic")
  enable_testing()
  add_subdirectory(tests)
else()
  message(NOTICE "CMAKE_SYSTEM_NAME is ${CMAKE_SYSTEM_NAME}, skipping tests")
endif()


