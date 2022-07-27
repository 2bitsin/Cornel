#include "packet_buffer.hpp"
#include "packet_io.hpp"
#include "error.hpp"
#include "packet_io_serial.hpp"
#include "packet_io_pipe.hpp"

auto packet_io_connect::serial(std::string_view target, serial_params_type params) -> std::unique_ptr<packet_io_interface>
{	
	return std::make_unique<packet_io_serial>(target, params);
}

auto packet_io_connect::pipe(std::string_view target, client_versus_server_type client_vs_server) -> std::unique_ptr<packet_io_interface>
{
	return std::make_unique<packet_io_pipe>(target, client_vs_server);
}

auto packet_io_connect::network(std::string_view address, uint8_t port, client_versus_server_type client_vs_server) -> std::unique_ptr<packet_io_interface>
{
	throw error_not_implemented(format_error, __FUNCSIG__);
	return nullptr;	
}
