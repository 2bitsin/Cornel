add_library(meta meta.cpp)
target_include_directories(meta PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(meta PROPERTIES CXX_STANDARD 23)
