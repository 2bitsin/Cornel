add_library(netboot_libc cstdio.cpp)
target_include_directories(netboot_libc PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(netboot_libc PRIVATE hardware utils)