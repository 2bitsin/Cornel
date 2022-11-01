add_library(cimpl 
  memmove.cpp 
  strlen.cpp 
  toupper.cpp 
  tolower.cpp 
  memcmp.cpp
  memchr.cpp
  sprintf_lite.cpp)

target_include_directories(cimpl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_compile_options(cimpl PRIVATE -fno-lto)