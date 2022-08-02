#include "v4_address.hpp"

v4_address::v4_address()
: v4_address(0)
{}

v4_address::v4_address(uint16_t port)
: v4_address(0, port)
{}

v4_address::v4_address(uint32_t address, uint16_t port)
:	m_addr{ address },
	m_port{ port }
{}

v4_address::v4_address(std::pair<uint32_t, uint16_t> const& address_port_pair)
: v4_address(address_port_pair.first, address_port_pair.second)
{}

v4_address::v4_address(std::string_view address, uint16_t port)
: v4_address(v4_parse_address(address), port)
{}

v4_address::v4_address(std::string_view address_and_port)
: v4_address(v4_parse_address_and_port(address_and_port))
{}

auto v4_address::port() const noexcept -> uint16_t { return m_port; }

auto v4_address::addr() const noexcept -> uint32_t { return m_addr; }

auto v4_address::net_port() const noexcept -> uint16_t { return host_to_net<uint16_t>(m_port); }

auto v4_address::net_addr() const noexcept -> uint32_t { return host_to_net<uint32_t>(m_addr); }

auto v4_address::to_string() const noexcept -> std::string
{
	using namespace std::string_literals;
	return v4_address_to_string(m_addr) + ":"s + std::to_string(m_port);
}

auto v4_address::make_udp() const -> udp_socket
{
	return udp_socket(*this);
}

auto v4_address::any(std::uint16_t port) -> v4_address
{
	return v4_address(0x00000000, port);
}

auto v4_address::everyone(std::uint16_t port) -> v4_address
{
	return v4_address(0xffffffff, port);
}
