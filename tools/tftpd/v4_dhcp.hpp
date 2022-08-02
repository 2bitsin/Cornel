#pragma once

#include <cstddef>
#include <cstdint>

#include <span>
#include <iostream>

struct v4_dhcp
{
	#pragma pack(push, 1)
	struct basic_info
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
	};
	#pragma pack(pop)


	enum opcode : uint8_t
	{
		REQUEST		= 1u,
		REPLY			= 2u
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
		ETHERNET	= 1u		
	};

	using mac_address_type = std::uint8_t[6];
	
	static auto parse(std::span<const std::byte> bits) -> v4_dhcp;

	auto pretty_print(std::ostream& oss) -> std::ostream&;

protected:
	static auto parse(v4_dhcp& target, std::span<const std::byte> bits) -> void;
	
private:
	basic_info m_info;
	
};