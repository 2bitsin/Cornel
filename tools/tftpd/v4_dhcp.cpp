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
		_serdes.deserialize(m_base);		
		target.m_base = std::move(m_base);	
		
		v4_dhcp_opts m_opts;
		_serdes.deserialize(m_opts);
		target.m_opts = std::move(m_opts);
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

auto v4_dhcp_message_type_to_string(uint8_t value) -> std::string
{
	using namespace std::string_literals;
	switch (value)
	{
	case v4_dhcp::DHCPDISCOVER	: return "DHCPDISCOVER"s;
	case v4_dhcp::DHCPOFFER			: return "DHCPOFFER"s;
	case v4_dhcp::DHCPRQUEST		: return "DHCPREQUEST"s;
	case v4_dhcp::DHCPDECLINE		: return "DHCPDECLINE"s;
	case v4_dhcp::DHCPACK				: return "DHCPACK"s;
	case v4_dhcp::DHCPNAK				: return "DHCPNAK"s;
	case v4_dhcp::DHCPRELEASE		: return "DHCPRELEASE"s;
	case v4_dhcp::DHCPINFORM		: return "DHCPINFORM"s;
	default: return "OTHER("s + std::to_string(value) + ")"s;
	}
}

auto v4_dhcp_system_architecture_to_string(uint16_t value) -> std::string
{
	using namespace std::string_literals;
	switch (value)
	{
	case 0: return "Intel x86PC"s;
  case 1: return "NEC/PC98"s;
  case 2: return "EFI Itanium"s;
  case 3: return "DEC Alpha"s;
  case 4: return "Arc x86"s;
  case 5: return "Intel Lean Client"s;
  case 6: return "EFI IA32"s;
  case 7: return "EFI BC"s;
  case 8: return "EFI Xscale"s;
  case 9: return "EFI x86-64"s;
	default: return "Other ("s + std::to_string(value) + ")"s;
	}
}

auto v4_dhcp_option_number_to_string(uint8_t id)
{
	using namespace std::string_literals;
	switch (id)
	{
	case 0: return "Pad"s;
	case 1: return "Subnet Mask"s;
	case 2: return "Time Offset"s;
	case 3: return "Router"s;
	case 4: return "Time Server"s;
	case 5: return "Name Server"s;
	case 6: return "Domain Name Server"s;
	case 7: return "Log Server"s;
	case 8: return "Quotes Server"s;
	case 9: return "LPR Server"s;
	case 10: return "Impress Server"s;
	case 11: return "RLP Server"s;
	case 12: return "Hostname"s;
	case 13: return "Boot File Size"s;
	case 14: return "Merit Dump File"s;
	case 15: return "Domain Name"s;
	case 16: return "Swap Server"s;
	case 17: return "Root Path"s;
	case 18: return "Extensions Path"s;
	case 19: return "IP Forwarding"s;
	case 20: return "Non-Local Source Routing"s;
	case 21: return "Policy Filter"s;
	case 22: return "Maximum Datagram Reassembly Size"s;
	case 23: return "Default IP Time-to-Live"s;
	case 24: return "Path MTU Aging Timeout"s;
	case 25: return "Path MTU Plateau Table"s;
	case 26: return "Interface MTU"s;
	case 27: return "All Subnets Local"s;
	case 28: return "Broadcast Address"s;
	case 29: return "Mask Discovery"s;
	case 30: return "Mask Supplier"s;
	case 31: return "Router Discovery"s;
	case 32: return "Router Request"s;
	case 33: return "Static Route"s;
	case 34: return "Trailer Encapsulation"s;
	case 35: return "ARP Cache Timeout"s;
	case 36: return "Ethernet Encapsulation"s;
	case 37: return "TCP Default TTL"s;
	case 38: return "TCP Keepalive Interval"s;
	case 39: return "TCP Keepalive Garbage"s;
	case 40: return "Network Information Service Domain"s;
	case 41: return "Network Information Service Servers"s;
	case 42: return "Network Time Protocol Servers"s;
	case 43: return "Vendor Specific Information"s;
	case 44: return "NetBIOS over TCP/IP Name Server"s;
	case 45: return "NetBIOS over TCP/IP Datagram Distribution Server"s;
	case 46: return "NetBIOS over TCP/IP Node Type"s;
	case 47: return "NetBIOS over TCP/IP Scope"s;
	case 48: return "X Window System Font Server"s;
	case 49: return "X Window System Display Manager"s;
	case 50: return "Requested IP Address"s;
	case 51: return "IP Address Lease Time"s;
	case 52: return "Option Overload"s;
	case 53: return "DHCP Message Type"s;
	case 54: return "DHCP Server Identifier"s;
	case 55: return "Parameter Request List"s;
	case 56: return "Message"s;
	case 57: return "Maximum DHCP Message Size"s;
	case 58: return "Renewal Time"s;
	case 59: return "Rebinding Time"s;
	case 60: return "Vendor Class Identifier"s;
	case 61: return "Client Identifier"s;
	case 62: return "NetWare/IP Domain Name"s;
	case 63: return "NetWare/IP Information"s;
	case 64: return "NIS Domain Name"s;
	case 65: return "NIS Servers"s;
	case 66: return "TFTP server name"s;
	case 67: return "Boot filename"s;
	case 68: return "Mobile IP Home Agent"s;
	case 69: return "SMTP server"s;
	case 70: return "POP3 server"s;
	case 71: return "NNTP server"s;
	case 72: return "WWW server"s;
	case 73: return "Finger server"s;
	case 74: return "IRC server"s;
	case 75: return "StreetTalk server"s;
	case 76: return "StreetTalk Directory Assistance server"s;
	case 77: return "User Class"s;
	case 78: return "Directory Agent"s;
	case 79: return "Service Scope"s;
	case 80: return "Rapid Commit"s;
	case 81: return "Client FQDN"s;
	case 82: return "Relay Agent Information"s;
	case 83: return "iSNS"s;
	case 84: return "Unassigned/Rremoved"s;
	case 85: return "NDS Servers"s;
	case 86: return "NDS Tree Name"s;
	case 87: return "NDS Context"s;
	case 88: return "BCMCS Controller Domain Name List"s;
	case 89: return "BCMCS Controller IPv4 Address List"s;
	case 90: return "Authentication"s;
	case 91: return "Client Last Transaction Time"s;	
	case 92: return "associated-ip"s;
	case 93: return "Client System"s;
	case 94: return "Client NDI"s;
	case 95: return "LDAP"s;
	case 96: return "Unassigned/Removed"s;
	case 97: return "UUID/GUID based client identifier"s;
	case 98: return "User Authentication"s;
	case 99: return "GEOCONF_CIVIC"s;
	default: return "Other ("s + std::to_string(id) + ")"s;			
	}
}


auto v4_dhcp::pretty_print_option(std::ostream& oss, v4_dhcp_opts::option<v4_dhcp_opts::CODE_MESSAGE_TYPE> const& item) -> std::ostream&
{
	using namespace std::string_view_literals;
	oss << std::format(" * DHCP message type ..... : {} \n"sv, v4_dhcp_message_type_to_string(item.value));
	return oss;
}

auto v4_dhcp::pretty_print_option(std::ostream& oss, v4_dhcp_opts::option<v4_dhcp_opts::CODE_CLIEANT_SYSTEM_ARCHITECTURE> const& item) -> std::ostream&
{
	using namespace std::string_view_literals;
	oss << std::format(" * Client architecture ... : {} \n"sv, v4_dhcp_system_architecture_to_string(item.value));
	return oss;
}

auto v4_dhcp::pretty_print_option(std::ostream& oss, v4_dhcp_opts::option<v4_dhcp_opts::CODE_PARAMETER_REQUEST_LIST> const& item) -> std::ostream&
{
	using namespace std::string_view_literals;
	oss << std::format(" * Required parameters ... :\n"sv);
	for (auto i = 0; i < item.length; ++i)
	{
		oss<<std::format("   - ({}) {}\n"sv, item.values[i], v4_dhcp_option_number_to_string(item.values[i]));
	}
	return oss;
}


template <typename T>
auto& v4_dhcp::pretty_print_option(std::ostream& oss, T const& value)
{
	return oss;
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
	
	
	for (auto&& [id, data] : m_opts.m_list)
	{
		oss<<std::format("Option ................... : {} \n"sv, id);	
		std::visit([this, &oss](auto&& item) {
			pretty_print_option(oss, item);
		}, data);
		
	}
	
	
	return oss;
}