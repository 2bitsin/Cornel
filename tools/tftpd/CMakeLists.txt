cmake_minimum_required(VERSION 3.18)
project(tftpd)
add_executable(tftpd 
	main.cpp 
	v4_address.hpp
	socket_api.hpp
	socket_api.cpp
	udp_socket.hpp
  udp_socket.cpp)

target_link_libraries(tftpd PRIVATE common)
set_property(TARGET tftpd PROPERTY CXX_STANDARD 23)

