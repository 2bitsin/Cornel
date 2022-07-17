#pragma once

#include <span>
#include <cstddef>
#include <cstdint>
#include <system_error>
#include <functional>

using byte = uint8_t;

template <typename T = byte>
struct packet_buffer
{	
	using packet_data			= std::span<T>;
	using ct_packet_data	= std::span<const T>;
	using deleter_type		= std::function<void(packet_data)>;

	packet_buffer(packet_data databits, deleter_type deleter);
	packet_buffer(std::size_t allocate);
	
	packet_buffer(packet_buffer&& other);
	packet_buffer& operator=(packet_buffer&& other);
 ~packet_buffer();

	void clone_into(packet_buffer& other);
	bool clone_into(packet_buffer& other, std::error_code& ec) noexcept;
	auto clone() -> packet_buffer;

	auto data() const noexcept -> ct_packet_data;
	auto data() noexcept -> packet_data;

	auto size() const noexcept -> std::size_t;	
	auto deleter() const noexcept -> deleter_type;
	auto deleter(deleter_type deleter) noexcept -> deleter_type;

	auto crc32() const noexcept -> std::uint32_t;

private:
	packet_data		m_databits;
	deleter_type	m_deleter;	
};
