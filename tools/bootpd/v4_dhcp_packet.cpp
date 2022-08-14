#include <string>
#include <array>
#include <format>

#include <common/socket_api.hpp>
#include <common/byte_order.hpp>
#include <common/utility_span.hpp>
#include <common/serdes.hpp>

#include "v4_dhcp_packet.hpp"

static auto v4_dhcp_opcode_to_string(std::uint8_t opcode)
	-> std::string
{
	using namespace std::string_literals;
	switch (opcode)
	{
	case 1:
		return "Request"s;
	case 2:
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


v4_dhcp_packet::v4_dhcp_packet()
{}

v4_dhcp_packet::v4_dhcp_packet(std::span<const std::byte> bits)
{ (::serdes<serdes_reader>(bits))(*this); }

v4_dhcp_packet::v4_dhcp_packet(::serdes<serdes_reader>& _serdes)
{ _serdes(*this); }

v4_dhcp_packet::v4_dhcp_packet(std::vector<std::byte> const& bits)
:	v4_dhcp_packet(std::span<const std::byte>{ bits })
{}

auto v4_dhcp_packet::serdes(::serdes<serdes_writer>& _serdes) const ->::serdes<serdes_writer>&
{
    _serdes(m_opcode);
    _serdes(m_hardware_type);
    _serdes(m_hardware_address_length);
    _serdes(m_number_of_hops);
    _serdes(m_transaction_id);
    _serdes(m_seconds_elapsed);
    _serdes(m_flags);
    _serdes(m_client_ip_address_v4);
    _serdes(m_your_ip_address_v4);
    _serdes(m_server_ip_address_v4);
    _serdes(m_gateway_ip_address_v4);
    _serdes(m_client_hardware_address);
    _serdes(m_server_host_name);
    _serdes(m_boot_file_name);
    _serdes(m_options);
    return _serdes;
}

auto v4_dhcp_packet::serdes(::serdes<serdes_reader>& _serdes) ->::serdes<serdes_reader>&
{
	_serdes(m_opcode);
	_serdes(m_hardware_type);
	_serdes(m_hardware_address_length);
	_serdes(m_number_of_hops);
	_serdes(m_transaction_id);
	_serdes(m_seconds_elapsed);
	_serdes(m_flags);
	_serdes(m_client_ip_address_v4);
	_serdes(m_your_ip_address_v4);
	_serdes(m_server_ip_address_v4);
	_serdes(m_gateway_ip_address_v4);
	_serdes(m_client_hardware_address);
	_serdes(m_server_host_name);
	_serdes(m_boot_file_name);
	_serdes(m_options);
	return _serdes;
}

auto v4_dhcp_packet::pretty_print(std::ostream& oss) const ->std::ostream&
{
	using namespace std::string_view_literals;
	oss << std::format("Opcode ................... : {} \n"sv, v4_dhcp_opcode_to_string(m_opcode));
	oss << std::format("Hardware type ............ : {} \n"sv, v4_dhcp_hardware_type_to_string(m_hardware_type));
	oss << std::format("Hardware address length .. : {} \n"sv, m_hardware_address_length);
	oss << std::format("Number of hops ........... : {} \n"sv, m_number_of_hops);
	oss << std::format("Transaction ID ........... : {:#08x} \n"sv, m_transaction_id);
	oss << std::format("Seconds elapsed .......... : {}s \n"sv, m_seconds_elapsed);
	oss << std::format("Bootp flags .............. : {} \n"sv, v4_dhcp_bootp_flags_to_string(m_flags));
	oss << std::format("Client IP address ........ : {} \n"sv, v4_address_to_string(m_client_ip_address_v4));
	oss << std::format("Your IP address .......... : {} \n"sv, v4_address_to_string(m_your_ip_address_v4));
	oss << std::format("Server IP address ........ : {} \n"sv, v4_address_to_string(m_server_ip_address_v4));
	oss << std::format("Gateway IP address ....... : {} \n"sv, v4_address_to_string(m_gateway_ip_address_v4));
	oss << std::format("Client hardware address .. : {} \n"sv, mac_address_to_string({m_client_hardware_address, m_hardware_address_length}));
	oss << std::format("Server host name ......... : '{}' \n"sv, std::string(m_server_host_name).c_str());
	oss << std::format("Boot file name ........... : '{}' \n"sv, std::string(m_boot_file_name).c_str());
	
	return oss;
}

auto v4_dhcp_packet::is_message_type(std::uint8_t msg_type) const -> bool
{ auto mto = m_options.message_type(); return mto.has_value() ? *mto == msg_type : false; }

auto v4_dhcp_packet::opcode() const -> std::uint8_t
{
	return m_opcode;
}

auto v4_dhcp_packet::hardware_type() const -> std::uint8_t
{
	return m_hardware_type;
}

auto v4_dhcp_packet::hardware_address_length() const -> std::uint8_t
{
	return m_hardware_address_length;
}

auto v4_dhcp_packet::hardware_address() const -> std::span<const std::uint8_t>
{
	return  { m_client_hardware_address, m_hardware_address_length };
}

auto v4_dhcp_packet::number_of_hops() const -> std::uint8_t
{
	return m_number_of_hops;
}

auto v4_dhcp_packet::client_address() const -> std::uint32_t
{
	return m_client_ip_address_v4;
}

auto v4_dhcp_packet::your_address() const -> std::uint32_t
{
	return m_your_ip_address_v4;
}

auto v4_dhcp_packet::server_address() const -> std::uint32_t
{
	return m_server_ip_address_v4;
}

auto v4_dhcp_packet::gateway_address() const -> std::uint32_t
{
	return m_gateway_ip_address_v4;
}

auto v4_dhcp_packet::transaction_id() const -> std::uint32_t
{
	return m_transaction_id;
}

auto v4_dhcp_packet::seconds_elapsed() const -> std::uint16_t
{
	return m_seconds_elapsed;
}

auto v4_dhcp_packet::flags() const -> std::uint16_t
{
	return m_flags;
}

auto v4_dhcp_packet::server_host_name() const -> std::string_view
{
	return { m_server_host_name, strnlen_s(m_server_host_name, sizeof(m_server_host_name)) };
}

auto v4_dhcp_packet::boot_file_name() const -> std::string_view
{
	return { m_boot_file_name, strnlen_s(m_boot_file_name, sizeof(m_boot_file_name)) };
}

auto v4_dhcp_packet::message_type() const -> std::optional<std::uint8_t>
{ return m_options.message_type(); }

auto v4_dhcp_packet::message_type(std::uint8_t msg_type) -> v4_dhcp_packet&
{ m_options.message_type(msg_type); return *this; }

auto v4_dhcp_packet::requested_parameters() const -> std::span<const std::uint8_t>
{ return m_options.requested_parameters(); }

auto v4_dhcp_packet::options() -> v4_dhcp_options&
{ return m_options; }

auto v4_dhcp_packet::options() const -> v4_dhcp_options const&
{ return m_options; }

auto v4_dhcp_packet::assign_options(v4_dhcp_options const& from, std::span<const std::uint8_t> which) -> v4_dhcp_packet&
{
	for (auto&& index : which)
		m_options.assign(index, from);
	return *this;
}

auto v4_dhcp_packet::assign_options(v4_dhcp_options const& from, std::initializer_list<const std::uint8_t> which) -> v4_dhcp_packet&
{
	return assign_options(from, std::span{ which });
}

auto v4_dhcp_packet::opcode(std::uint8_t value) -> v4_dhcp_packet&
{
	m_opcode = value; return *this;
}

auto v4_dhcp_packet::hardware_type(std::uint8_t value) -> v4_dhcp_packet&
{
	m_hardware_type = value; return *this;
}

auto v4_dhcp_packet::hardware_address(std::span<const std::uint8_t> value) -> v4_dhcp_packet&
{
	if (value.size() > sizeof(m_client_hardware_address))
		throw std::logic_error("hardware_address too large");
	m_hardware_address_length = value.size();
	std::copy(value.begin(), value.end(), m_client_hardware_address);
	return *this;
}

auto v4_dhcp_packet::client_address(std::uint32_t value) -> v4_dhcp_packet&
{
	m_client_ip_address_v4 = value; return *this;
}

auto v4_dhcp_packet::your_address(std::uint32_t value) -> v4_dhcp_packet&
{
	m_your_ip_address_v4 = value; return *this;
}

auto v4_dhcp_packet::server_address(std::uint32_t value) -> v4_dhcp_packet&
{
	m_server_ip_address_v4 = value; return *this;
}

auto v4_dhcp_packet::gateway_address(std::uint32_t value) -> v4_dhcp_packet&
{
	m_gateway_ip_address_v4 = value; return *this;
}

auto v4_dhcp_packet::transaction_id(std::uint32_t value) -> v4_dhcp_packet&
{
	m_transaction_id = value; return *this;
}

auto v4_dhcp_packet::seconds_elapsed(std::uint16_t value) -> v4_dhcp_packet&
{
	m_seconds_elapsed = value; return *this;
}

auto v4_dhcp_packet::flags(std::uint16_t value) -> v4_dhcp_packet&
{
	m_flags = value; return *this;
}

auto v4_dhcp_packet::number_of_hops(std::uint8_t hops) -> v4_dhcp_packet&
{
	m_number_of_hops = hops; return *this;
}

auto v4_dhcp_packet::server_host_name(std::string_view value) -> v4_dhcp_packet&
{
	if (value.size() > sizeof(m_server_host_name))
		throw std::logic_error("host_name too large");
	std::fill(std::begin(m_server_host_name), std::end(m_server_host_name), 0);
	std::copy(value.begin(), value.end(), m_server_host_name);
	return *this;
}

auto v4_dhcp_packet::boot_file_name(std::string_view value) -> v4_dhcp_packet&
{
	if (value.size() > sizeof(m_boot_file_name))
		throw std::logic_error("boot_file_name too large");
	std::fill(std::begin(m_boot_file_name), std::end(m_boot_file_name), 0);
	std::copy(value.begin(), value.end(), m_boot_file_name);
	return *this;
}

auto v4_dhcp_packet::serdes_size_hint() const -> std::size_t
{
	return
		m_options.serdes_size_hint()
		+ sizeof(m_opcode)
		+ sizeof(m_hardware_type)
		+ sizeof(m_hardware_address_length)
		+ sizeof(m_number_of_hops)
		+ sizeof(m_transaction_id)
		+ sizeof(m_seconds_elapsed)
		+ sizeof(m_flags)
		+ sizeof(m_client_ip_address_v4)
		+ sizeof(m_your_ip_address_v4)
		+ sizeof(m_server_ip_address_v4)
		+ sizeof(m_gateway_ip_address_v4)
		+ sizeof(m_client_hardware_address)
		+ sizeof(m_server_host_name)
		+ sizeof(m_boot_file_name);
}
