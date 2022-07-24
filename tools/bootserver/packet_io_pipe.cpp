#include "packet_io_pipe.hpp"

packet_io_pipe::packet_io_pipe(std::string_view target, client_vs_server_type cvs)
:	packet_io_win32(use_pipe, target, cvs)
{}
