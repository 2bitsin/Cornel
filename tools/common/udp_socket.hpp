#pragma once

#include <span>
#include <chrono>

inline static const constexpr std::uint32_t message_out_of_bounds_flag	= 0x01u;
inline static const constexpr std::uint32_t message_peek_flag						= 0x02u;
inline static const constexpr std::uint32_t message_dont_route_flag			= 0x04u;
inline static const constexpr std::uint32_t message_wait_all_flag				= 0x08u;

#include "socket_api.hpp"

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

	auto recv(uint32_t flags) -> std::tuple<v4_address, std::vector<std::byte>>;
	

	template <typename O>
	auto option(const typename O::value_type& value) const -> void
	{
		return socket_option<O>(m_sock, value);
	}

	template <typename O>
	auto option() const -> typename O::value_type
	{
		return socket_option<O>(m_sock);
	}

	template <typename... D>
	void timeout_recv(std::chrono::duration<D...> const& dur)
	{
		using namespace std::chrono;
		const auto to = duration_cast<milliseconds>(dur);
		option<so_rcvtimeo>((std::uint32_t)to.count());		
	}
		
	template <typename... D>
	void timeout_send(std::chrono::duration<D...> const& dur)
	{
		using namespace std::chrono;
		const auto to = duration_cast<milliseconds>(dur);
		option<so_sndtimeo>((std::uint32_t)to.count());		
	}

	template <typename... D>
	void timeout(std::chrono::duration<D...> const& dur)
	{
		using namespace std::chrono;
		const auto to = duration_cast<milliseconds>(dur);
		option<so_rcvtimeo>((std::uint32_t)to.count());
		option<so_sndtimeo>((std::uint32_t)to.count());		
	}

protected:
	udp_socket(int_socket_type int_sock);
private:
	int_socket_type m_sock;
};