add_library(pxenv core.cpp dhcp.cpp tftp.cpp tftp_notify.cpp)
target_include_directories(pxenv PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(pxenv PUBLIC hardware textio netboot)