#pragma once

#include <cstddef>
#include <cstdint>

#include <iostream>
#include <span>
#include <optional>

#include <common/serdes.hpp>
#include "v4_dhcp_options.hpp"

struct v4_dhcp_packet
{

	using mac_address_type = std::uint8_t[6];	


	template <auto... U>
	v4_dhcp_packet(serdes<U...>& _serdes)
	{
		_serdes(*this);
	}

	v4_dhcp_packet()
	{}


	template <typename _Serdes>
	auto serdes(_Serdes& _serdes) -> _Serdes&
	{
		SERDES_APPLY(_serdes, m_opcode);
		SERDES_APPLY(_serdes, m_hardware_type);
		SERDES_APPLY(_serdes, m_hardware_address_length);
		SERDES_APPLY(_serdes, m_number_of_hops);
		SERDES_APPLY(_serdes, m_transaction_id);
		SERDES_APPLY(_serdes, m_seconds_elapsed);
		SERDES_APPLY(_serdes, m_flags);
		SERDES_APPLY(_serdes, m_client_ip_address_v4);
		SERDES_APPLY(_serdes, m_your_ip_address_v4);
		SERDES_APPLY(_serdes, m_server_ip_address_v4);
		SERDES_APPLY(_serdes, m_gateway_ip_address_v4);
		SERDES_APPLY(_serdes, m_client_hardware_address);
		SERDES_APPLY(_serdes, m_server_host_name);
		SERDES_APPLY(_serdes, m_boot_file_name);
		SERDES_APPLY(_serdes, m_magic_cookie);
		SERDES_APPLY(_serdes, m_options);
		return _serdes;
	}

	auto pretty_print(std::ostream& oss) -> std::ostream&;

protected:

	// Primary
	std::uint8_t		m_opcode;
	std::uint8_t		m_hardware_type;
	std::uint8_t		m_hardware_address_length;
	std::uint8_t		m_number_of_hops;
	std::uint32_t		m_transaction_id;
	std::uint16_t		m_seconds_elapsed;
	std::uint16_t		m_flags;
	std::uint32_t		m_client_ip_address_v4;
	std::uint32_t		m_your_ip_address_v4;
	std::uint32_t		m_server_ip_address_v4;
	std::uint32_t		m_gateway_ip_address_v4;
	std::uint8_t		m_client_hardware_address [16];
	char						m_server_host_name [64];
	char						m_boot_file_name [128];	
	std::uint32_t		m_magic_cookie;
	v4_dhcp_options m_options;
};