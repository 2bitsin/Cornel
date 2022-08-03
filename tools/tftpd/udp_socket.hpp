#pragma once

#include <span>

inline static const constexpr std::uint32_t message_out_of_bounds_flag	= 0x01u;
inline static const constexpr std::uint32_t message_peek_flag						= 0x02u;
inline static const constexpr std::uint32_t message_dont_route_flag			= 0x04u;
inline static const constexpr std::uint32_t message_wait_all_flag				= 0x08u;


struct udp_socket
{
	udp_socket();
	udp_socket(const struct v4_address& addr);
	udp_socket(udp_socket&& from);
	udp_socket& operator = (udp_socket && from);
	udp_socket(const udp_socket&) = delete;
	udp_socket& operator = (const udp_socket&) = delete;
 ~udp_socket();
  void swap(udp_socket& other);
	void bind(const struct v4_address& addr);
	
	/* buffer will be adjusted to span only the bytes received */
	auto recv(std::span<std::byte>& buffer, struct v4_address& source, uint32_t flags) -> std::size_t;

	/* buffer will be adjusted to span only the bytes not sent */
	auto send(std::span<const std::byte>& buffer, const struct v4_address& target, uint32_t flags) -> std::size_t;
protected:
	udp_socket(int_socket_type int_sock);
private:
	int_socket_type m_sock;
};