#include "packet_buffer.hpp"
#include "packet_io.hpp"
#include "error.hpp"
#include "packet_io_pipe.hpp"


auto packet_io_connect::serial(std::string_view port, serial_params_type params) -> std::unique_ptr<Ipacket_io>
{
	throw error_not_implemented(format_error, __FUNCSIG__);
	return nullptr;
}

auto packet_io_connect::pipe(std::string_view name, client_vs_server_type client_vs_server) -> std::unique_ptr<Ipacket_io>
{
	throw error_not_implemented(format_error, __FUNCSIG__);
	return nullptr;
}

auto packet_io_connect::network(std::string_view address, uint8_t port, client_vs_server_type client_vs_server) -> std::unique_ptr<Ipacket_io>
{
	throw error_not_implemented(format_error, __FUNCSIG__);
	return nullptr;	
}
