cmake_minimum_required(VERSION 3.18)
project(bootpd)
add_executable(bootpd	
	main.cpp 
  v4_dhcp_packet.hpp
  v4_dhcp_packet.cpp
	v4_dhcp_server.hpp	
	v4_dhcp_server.cpp	
	v4_dhcp_options.hpp
  v4_tftp_server.cpp
	v4_tftp_server.hpp
)

target_link_libraries(bootpd PRIVATE common)
set_property(TARGET bootpd PROPERTY CXX_STANDARD 23)


