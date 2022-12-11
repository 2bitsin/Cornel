add_library(cimpl 
  memchr.cpp
  memcmp.cpp
  memmove.cpp 
  strlen.cpp 
  toupper.cpp 
  tolower.cpp 
  sprintf_lite.cpp)

target_compile_options(cimpl PRIVATE "-fno-lto")
target_include_directories(cimpl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(cimpl PRIVATE)