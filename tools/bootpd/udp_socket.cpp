#include "v4_address.hpp"
#include "socket_api.hpp"
#include "udp_socket.hpp"

#include <vector>
#include <array>

using std::exchange;

udp_socket::udp_socket()
:	udp_socket(v4_socket_make_invalid())
{}

udp_socket::udp_socket(int_socket_type int_sock)
:	m_sock{ int_sock }
{}

udp_socket::udp_socket(const struct v4_address& addr)
:	udp_socket(v4_socket_make_udp(addr))
{}

udp_socket::udp_socket(udp_socket&& from)
:	m_sock{ exchange(from.m_sock, v4_socket_make_invalid()) }
{}

auto udp_socket::operator = (udp_socket&& from) -> udp_socket&
{
	udp_socket tmp(std::move(from));
	tmp.swap(*this);
	return *this;
}

udp_socket::~udp_socket()
{
	if (m_sock != v4_socket_make_invalid())
		v4_socket_close(m_sock);
}

void udp_socket::swap(udp_socket& other)
{
	std::swap(other.m_sock, m_sock);
}

void udp_socket::bind(const v4_address& addr)
{
	v4_socket_bind(m_sock, addr);
}

auto udp_socket::recv(std::span<std::byte>& buffer, v4_address& source, uint32_t flags) -> std::size_t
{
	return v4_socket_recv(m_sock, buffer, source, flags);
}

auto udp_socket::send(std::span<const std::byte>& buffer, const v4_address& target, uint32_t flags) -> std::size_t
{
	return v4_socket_send(m_sock, buffer, target, flags);
}

auto udp_socket::recv(uint32_t flags) -> std::tuple<v4_address, std::vector<std::byte>>
{
  thread_local std::array<std::byte, 0x10000u> array_buffer;
	std::span<std::byte> buffer_s{ array_buffer };
	v4_address source;
	if ((*this).recv(buffer_s, source, flags) > 0u)
	{
		std::vector<std::byte> received_bytes(buffer_s.begin(), buffer_s.end());
		return std::tuple(std::move(source), std::move(received_bytes));
	}		
	throw std::runtime_error("recv failed");
}
