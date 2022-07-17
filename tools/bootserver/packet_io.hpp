#pragma once

#include <memory>
#include "packet_buffer.hpp"

struct Ipacket_io
{
	virtual void send(const packet_buffer<byte>& buffer) = 0;
	virtual auto recv() -> packet_buffer<byte> = 0;	
};

struct packet_io_connect
{	
	enum parity_type
	{
		parity_none,
		parity_even,
		parity_odd,
		parity_space,
		parity_mark
	};

	enum client_vs_server_type
	{
		as_client,
		as_server
	};

	struct serial_params_type
	{
		uint32_t baud_rate { 115200 };
		uint8_t data_bits { 8 };
		uint8_t stop_bits { 1 };
		parity_type parity { parity_none };
	};
	
	static auto serial(std::string_view port, serial_params_type params = serial_params_type{})->std::unique_ptr<Ipacket_io>;
	static auto pipe(std::string_view name, client_vs_server_type client_vs_server = as_client) -> std::unique_ptr<Ipacket_io>;
	static auto network(std::string_view address, uint8_t port, client_vs_server_type client_vs_server = as_client)	-> std::unique_ptr<Ipacket_io>;
};