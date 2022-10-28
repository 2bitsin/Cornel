add_library(impl cstdio.cpp)
target_include_directories(impl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(impl PRIVATE hardware utils)