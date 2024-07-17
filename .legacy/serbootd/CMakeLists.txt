cmake_minimum_required(VERSION 3.18)
project(serbootd)

add_library(serboot STATIC
	error.hpp
	command_line.hpp
	command_line.cpp
	packet_buffer.hpp
	packet_buffer.cpp
	packet_io.hpp
  packet_io.cpp
	packet_io_win32.hpp
	packet_io_win32.cpp
	packet_io_pipe.hpp
	packet_io_pipe.cpp
	packet_io_serial.hpp
	packet_io_serial.cpp
)
	
set_property(TARGET serboot PROPERTY CXX_STANDARD 23)
add_executable(serbootd server_main.cpp)
target_link_libraries(serbootd PRIVATE common serboot)
set_property(TARGET serbootd PROPERTY CXX_STANDARD 23)

