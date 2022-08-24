add_library(libcimpl memmove.c)
target_include_directories(libcimpl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})