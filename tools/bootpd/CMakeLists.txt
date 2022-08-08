cmake_minimum_required(VERSION 3.18)
project(bootpd)
add_executable(bootpd	
	main.cpp 
	socket_api.hpp
	socket_api.cpp
	v4_address.hpp
	v4_address.cpp
	udp_socket.hpp
  udp_socket.cpp 
  v4_dhcp_packet.hpp
  v4_dhcp_packet.cpp
	socket_option.hpp
  socket_option.cpp "v4_dhcp_listener.cpp")

target_link_libraries(bootpd PRIVATE common)
set_property(TARGET bootpd PROPERTY CXX_STANDARD 23)

