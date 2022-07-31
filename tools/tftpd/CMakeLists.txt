cmake_minimum_required(VERSION 3.18)
project(tftpd)
add_executable(tftpd 
	main.cpp 
	socket_api.hpp
	socket_api.cpp
	v4_address.hpp
	udp_socket.hpp
)

target_link_libraries(tftpd PRIVATE common)
set_property(TARGET tftpd PROPERTY CXX_STANDARD 23)

