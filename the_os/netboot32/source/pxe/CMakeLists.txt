add_library(pxe pxe.cpp dhcp.cpp)
target_include_directories(pxe PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(pxe PUBLIC hardware textio netboot)