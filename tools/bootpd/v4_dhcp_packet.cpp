#include <string>
#include <array>
#include <format>

#include "socket_api.hpp"
#include "v4_dhcp_packet.hpp"
#include <common/byte_order.hpp>
#include <common/span_utility.hpp>
#include <common/serdes.hpp>




static auto v4_dhcp_opcode_to_string(std::uint8_t opcode)
	-> std::string
{
	using namespace std::string_literals;
	switch (opcode)
	{
	case 0:
		return "Request"s;
	case 1:
		return "Reply"s;
	default:
		return "Other ("s + std::to_string(opcode) + ")"s;
	}
}

static auto v4_dhcp_hardware_type_to_string(std::uint8_t htype)
	-> std::string
{
	using namespace std::string_literals;
	switch (htype)
	{
	case 1:
		return "Ethernet "s;
	default:
		return "Other ("s + std::to_string(htype) + ")"s;
	}
}

static auto v4_dhcp_bootp_flags_to_string(std::uint16_t flags)
	-> std::string
{
	using namespace std::string_literals;
	if (flags & 0x8000u)
		return "Broadcast"s;
	return ""s;
}


auto v4_dhcp_packet::pretty_print(std::ostream& oss)->std::ostream&
{
	using namespace std::string_view_literals;
	oss << std::format("Opcode ................... : {} \n"sv, v4_dhcp_opcode_to_string(m_opcode));
	oss << std::format("Hardware type ............ : {} \n"sv, v4_dhcp_hardware_type_to_string(m_hardware_type));
	oss << std::format("Hardware address length .. : {} \n"sv, m_hardware_address_length);
	oss << std::format("Number of hops ........... : {} \n"sv, m_number_of_hops);
	oss << std::format("Transaction ID ........... : {:#08x} \n"sv, m_transaction_id);
	oss << std::format("Seconds elapsed .......... : {} \n"sv, m_seconds_elapsed);
	oss << std::format("Bootp flags .............. : {} \n"sv, v4_dhcp_bootp_flags_to_string(m_flags));
	oss << std::format("Client IP address ........ : {} \n"sv, v4_address_to_string(m_client_ip_address_v4));
	oss << std::format("Your IP address .......... : {} \n"sv, v4_address_to_string(m_your_ip_address_v4));
	oss << std::format("Server IP address ........ : {} \n"sv, v4_address_to_string(m_server_ip_address_v4));
	oss << std::format("Gateway IP address ....... : {} \n"sv, v4_address_to_string(m_gateway_ip_address_v4));
	oss << std::format("Client hardware address .. : {} \n"sv, mac_address_to_string({m_client_hardware_address, m_hardware_address_length}));
	oss << std::format("Server host name ......... : {} \n"sv, std::string(m_server_host_name).c_str());
	oss << std::format("Boot file name ........... : {} \n"sv, std::string(m_boot_file_name).c_str());
	
	return oss;
}
