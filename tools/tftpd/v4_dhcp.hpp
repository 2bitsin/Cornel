#pragma once

#include <cstddef>
#include <cstdint>

#include <span>
#include <iostream>

#include "v4_dhcp_base.hpp"
#include "v4_dhcp_opts.hpp"

struct v4_dhcp
{
	enum opcode : uint8_t
	{
		REQUEST				= 1u,
		REPLY					= 2u
	};
	
	enum message_type : uint8_t
	{
		DHCPDISCOVER	= 1u,
		DHCPOFFER			= 2u,
		DHCPRQUEST		= 3u,
		DHCPDECLINE		= 4u,
		DHCPACK				= 5u,
		DHCPNAK				= 6u,
		DHCPRELEASE		= 7u,
		DHCPINFORM		= 8u
	};

	enum hardware_type : uint8_t
	{
		ETHERNET			= 1u		
	};

	using mac_address_type = std::uint8_t[6];
	
	static auto parse(std::span<const std::byte> bits) -> v4_dhcp;


	auto pretty_print(std::ostream& oss) -> std::ostream&;

protected:
	static auto parse(v4_dhcp& target, std::span<const std::byte> bits) -> void;

	auto pretty_print_option(std::ostream& oss, v4_dhcp_opts::option<v4_dhcp_opts::CODE_MESSAGE_TYPE> const& item) -> std::ostream&;
	auto pretty_print_option(std::ostream& oss, v4_dhcp_opts::option<v4_dhcp_opts::CODE_CLIEANT_SYSTEM_ARCHITECTURE> const& item) -> std::ostream&;
	auto pretty_print_option(std::ostream& oss, v4_dhcp_opts::option<v4_dhcp_opts::CODE_PARAMETER_REQUEST_LIST> const& item) -> std::ostream&;

	template<typename T>
	auto& pretty_print_option(std::ostream& oss, T const& value);
	
private:
	v4_dhcp_base m_base;
	v4_dhcp_opts m_opts;
};