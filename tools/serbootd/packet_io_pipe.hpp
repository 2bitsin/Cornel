#include "packet_io_win32.hpp"

struct packet_io_pipe
:	packet_io_win32
{
	packet_io_pipe (std::string_view target, client_versus_server_type cvs);
};