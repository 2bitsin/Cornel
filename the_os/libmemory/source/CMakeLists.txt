add_library(memory block_list.cpp)
target_include_directories(memory PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(memory PROPERTIES CXX_STANDARD 23)