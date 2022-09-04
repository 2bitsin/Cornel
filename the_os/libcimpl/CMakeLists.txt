add_library(cimpl memmove.cpp strlen.cpp toupper.cpp)
target_include_directories(cimpl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})