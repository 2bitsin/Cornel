#pragma once

#include <cstdint>
#include <cstddef>
#include <string_view>

#include "byteorder.hpp"
#include "socket_api.hpp"

struct v4_address
{
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
	
	auto net_port() const noexcept -> uint16_t { return host_to_net<uint32_t>(m_port); }
	auto net_addr() const noexcept -> uint32_t { return host_to_net<uint32_t>(m_addr); }
	
	template <typename T>
	auto assign_to(T& s) const
	{
		init_sockaddr(s, sizeof(s), *this);
	}	
	
	auto to_string() const noexcept
	{	
		using namespace std::string_literals;
		return v4_address_to_string(m_addr) + ":"s + std::to_string(m_port);
	}

private:
	uint32_t m_addr{ 0 } ;
	uint16_t m_port{ 0 } ;
};