#pragma once

#include "packet_io_win32.hpp"

struct packet_io_serial:
	public packet_io_win32
{
	packet_io_serial(std::string_view target, serial_params_type const& params); 
};