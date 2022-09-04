cmake_minimum_required(VERSION 3.23.0)
project(libtextio LANGUAGES C CXX)

add_subdirectory(source)

if(NOT (${CMAKE_SYSTEM_NAME} EQUAL "Generic"))
	enable_testing()
  add_subdirectory(tests)
endif()


