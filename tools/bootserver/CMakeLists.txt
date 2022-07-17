cmake_minimum_required(VERSION 3.18)
project(bootserver)

add_library(bootserver_library STATIC
	error.hpp
	command_line.hpp
	command_line.cpp
	packet_buffer.hpp
	packet_buffer.cpp
	packet_io.hpp
  packet_io.cpp
	packet_io_pipe.hpp
	packet_io_pipe.cpp)
set_property(TARGET bootserver_library PROPERTY CXX_STANDARD 23)

add_executable(bootserver server_main.cpp)
target_link_libraries(bootserver PRIVATE common bootserver_library)
set_property(TARGET bootserver PROPERTY CXX_STANDARD 23)

add_executable(bootserver_client client_main.cpp)
target_link_libraries(bootserver_client PRIVATE common bootserver_library)
set_property(TARGET bootserver_client PROPERTY CXX_STANDARD 23)

