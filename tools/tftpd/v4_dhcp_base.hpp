#pragma once

#include <cstddef>
#include <cstdint>

#include "common/serdes.hpp"

struct v4_dhcp_base
{
	std::uint8_t	opcode;
	std::uint8_t	hardware_type;
	std::uint8_t	hardware_address_length;
	std::uint8_t	number_of_hops;
	std::uint32_t transaction_id;
	std::uint16_t seconds_elapsed;
	std::uint16_t flags;
	std::uint32_t client_ip_address_v4;
	std::uint32_t your_ip_address_v4;
	std::uint32_t server_ip_address_v4;
	std::uint32_t gateway_ip_address_v4;
	std::uint8_t	client_hardware_address [16];
	char					server_host_name [64];
	char					boot_file_name [128];	
	std::uint32_t	magic_cookie;

	v4_dhcp_base() 
	{}

	template <auto ... Q>
	auto& deserialize(serdes<serdes_reader, Q...>& _serdes)
	{
		_serdes.deserialize(opcode);
		_serdes.deserialize(hardware_type);
		_serdes.deserialize(hardware_address_length);
		_serdes.deserialize(number_of_hops);
		_serdes.deserialize(transaction_id);
		_serdes.deserialize(seconds_elapsed);
		_serdes.deserialize(flags);
		_serdes.deserialize(client_ip_address_v4);
		_serdes.deserialize(your_ip_address_v4);
		_serdes.deserialize(server_ip_address_v4);
		_serdes.deserialize(gateway_ip_address_v4);
		_serdes.deserialize(client_hardware_address);
		_serdes.deserialize(server_host_name);
		_serdes.deserialize(boot_file_name);
		_serdes.deserialize(magic_cookie);
		return *this;
	}
};