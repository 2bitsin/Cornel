#pragma once

#include <cstdint>
#include <cstddef>
#include <string_view>
#include <string>

#include "byteorder.hpp"
#include "socket_api.hpp"
#include "udp_socket.hpp"

struct v4_address
{
	v4_address()
	: v4_address(0)
	{}

	v4_address(uint16_t port)
	: v4_address(0, port)
	{}

	v4_address(uint32_t address, uint16_t port)
	:	m_addr{ address },
		m_port{ port }
	{}

	v4_address(std::pair<uint32_t, uint16_t> const& address_port_pair)
	: v4_address(address_port_pair.first, address_port_pair.second)
	{}

	v4_address(std::string_view address, uint16_t port)
	:	v4_address(v4_parse_address(address), port)
	{}
	
	v4_address(std::string_view address_and_port)
	:	v4_address(v4_parse_address_and_port(address_and_port))
	{}
	
	auto port() const noexcept -> uint16_t { return m_port; }
	auto addr() const noexcept -> uint32_t { return m_addr; }
	
	auto net_port() const noexcept -> uint16_t { return host_to_net<uint16_t>(m_port); }
	auto net_addr() const noexcept -> uint32_t { return host_to_net<uint32_t>(m_addr); }
	
	template <typename T>
	auto assign_to(T& s) const
	{
		v4_init_sockaddr(s, sizeof(s), *this);
	}	

	template <typename T>
	auto as() const -> T
	{
		T target;
		assign_to(target);
		return target;
	}
	
	auto to_string() const noexcept
	{	
		using namespace std::string_literals;
		return v4_address_to_string(m_addr) + ":"s + std::to_string(m_port);
	}

	template <typename T>
	auto assign_from(T const& source)
	{
		auto[addr, port] = v4_parse_address_and_port(source);
		m_addr = addr;
		m_port = port;
	}

	auto make_udp () const -> udp_socket
	{
		return udp_socket(*this);
	}

	static auto any(std::uint16_t port = 0) -> v4_address
	{
		return v4_address(0x00000000, port);
	}

	static auto everyone(std::uint16_t port = 0) -> v4_address
	{
		return v4_address(0xffffffff, port);
	}

private:
	uint32_t m_addr{ 0 } ;
	uint16_t m_port{ 0 } ;
};