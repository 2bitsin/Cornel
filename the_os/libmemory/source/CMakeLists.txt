add_library(memory allocator.cpp)
target_include_directories(memory PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(memory PROPERTIES CXX_STANDARD 23)