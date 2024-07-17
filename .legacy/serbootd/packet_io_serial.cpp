#include "packet_io_serial.hpp"

packet_io_serial::packet_io_serial(std::string_view target, serial_params_type const& params)
:	packet_io_win32(use_serial, target, params)
{
}
