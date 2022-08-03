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

	template <is_required_type Is_required = required, auto ... Q>
	auto deserialize(serdes<serdes_reader, Q...>& _serdes)
	{
		_serdes.deserialize<Is_required, network_byte_order>(opcode);
		_serdes.deserialize<Is_required, network_byte_order>(hardware_type);
		_serdes.deserialize<Is_required, network_byte_order>(hardware_address_length);
		_serdes.deserialize<Is_required, network_byte_order>(number_of_hops);
		_serdes.deserialize<Is_required, network_byte_order>(transaction_id);
		_serdes.deserialize<Is_required, network_byte_order>(seconds_elapsed);
		_serdes.deserialize<Is_required, network_byte_order>(flags);
		_serdes.deserialize<Is_required, network_byte_order>(client_ip_address_v4);
		_serdes.deserialize<Is_required, network_byte_order>(your_ip_address_v4);
		_serdes.deserialize<Is_required, network_byte_order>(server_ip_address_v4);
		_serdes.deserialize<Is_required, network_byte_order>(gateway_ip_address_v4);
		_serdes.deserialize<Is_required, network_byte_order>(client_hardware_address);
		_serdes.deserialize<Is_required, network_byte_order>(server_host_name);
		_serdes.deserialize<Is_required, network_byte_order>(boot_file_name);
		_serdes.deserialize<Is_required, network_byte_order>(magic_cookie);
	}
};