#include <string>
#include <array>
#include <format>

#include "socket_api.hpp"
#include "v4_dhcp.hpp"
#include "common/byte_order.hpp"
#include "common/span_utility.hpp"
#include "common/serdes.hpp"

auto v4_dhcp::parse(std::span<const std::byte> bits) -> v4_dhcp
{
	v4_dhcp payload;
	v4_dhcp::parse(payload, bits);
	return payload;
}

auto v4_dhcp::parse(v4_dhcp& target, std::span<const std::byte> bits) -> void
{
	using namespace std::string_literals;	
	try
	{		
		serdes<serdes_reader, network_byte_order> _serdes(bits);
		v4_dhcp_base m_base;
		//_serdes.deserialize(m_base);
		m_base.deserialize(_serdes);
		target.m_base = std::move(m_base);		
	}	
	catch (std::exception const& ex)
	{
		throw std::runtime_error("can't parse dhcp packet, reason : "s + ex.what());
	}
}

static auto v4_dhcp_opcode_to_string(std::uint8_t opcode)
	-> std::string
{
	using namespace std::string_literals;
	switch (opcode)
	{
	case v4_dhcp::REQUEST:
		return "REQUEST"s;
	case v4_dhcp::REPLY:
		return "REPLY"s;
	default:
		return "OTHER("s + std::to_string(opcode) + ")"s;
	}
}

static auto v4_dhcp_hardware_type_to_string(std::uint8_t htype)
	-> std::string
{
	using namespace std::string_literals;
	switch (htype)
	{
	case v4_dhcp::ETHERNET:
		return "ETHERNET"s;
	default:
		return "OTHER("s + std::to_string(htype) + ")"s;
	}
}

static auto v4_dhcp_bootp_flags_to_string(std::uint16_t flags)
	-> std::string
{
	using namespace std::string_literals;
	if (flags & 0x8000u)
		return "BROADCAST"s;
	return ""s;
}

auto v4_dhcp::pretty_print(std::ostream& oss)->std::ostream&
{
	using namespace std::string_view_literals;
	oss << std::format("Opcode ................... : {} \n"sv, v4_dhcp_opcode_to_string(m_base.opcode));
	oss << std::format("Hardware type ............ : {} \n"sv, v4_dhcp_hardware_type_to_string(m_base.hardware_type));
	oss << std::format("Hardware address length .. : {} \n"sv, m_base.hardware_address_length);
	oss << std::format("Number of hops ........... : {} \n"sv, m_base.number_of_hops);
	oss << std::format("Transaction ID ........... : {:#08x} \n"sv, m_base.transaction_id);
	oss << std::format("Seconds elapsed .......... : {} \n"sv, m_base.seconds_elapsed);
	oss << std::format("Bootp flags .............. : {} \n"sv, v4_dhcp_bootp_flags_to_string(m_base.flags));
	oss << std::format("Client IP address ........ : {} \n"sv, v4_address_to_string(m_base.client_ip_address_v4));
	oss << std::format("Your IP address .......... : {} \n"sv, v4_address_to_string(m_base.your_ip_address_v4));
	oss << std::format("Server IP address ........ : {} \n"sv, v4_address_to_string(m_base.server_ip_address_v4));
	oss << std::format("Gateway IP address ....... : {} \n"sv, v4_address_to_string(m_base.gateway_ip_address_v4));
	oss << std::format("Client hardware address .. : {} \n"sv, mac_address_to_string({m_base.client_hardware_address, m_base.hardware_address_length}));
	oss << std::format("Server host name ......... : {} \n"sv, std::string(m_base.server_host_name).c_str());
	oss << std::format("Boot file name ........... : {} \n"sv, std::string(m_base.boot_file_name).c_str());
	oss << std::format("Magic cookie ............. : {:#08x} \n"sv, m_base.magic_cookie);
	
	
	return oss;
}