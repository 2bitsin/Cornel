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
	struct clone_from_t {};
	static inline const constexpr clone_from_t copy_from_flag {};
	
	using packet_data			= std::span<T>;
	using ct_packet_data	= std::span<const T>;
	using deleter_type		= std::function<void(packet_data)>;

	packet_buffer();
	packet_buffer(packet_data databits, deleter_type deleter);
	packet_buffer(std::size_t allocate);
	packet_buffer(clone_from_t, ct_packet_data databits);
	
	packet_buffer(packet_buffer&& other);
	packet_buffer& operator=(packet_buffer&& other);
 ~packet_buffer();

	auto copy_from(std::span<const T> bits) ->void;	
	auto copy_from(std::string_view bits) ->void;

	auto copy_into(packet_buffer& other) -> void;
	auto copy_into(packet_buffer& other, std::error_code& ec) noexcept -> bool;
	auto clone() -> packet_buffer;

	auto fill(T value) -> void;
	
	auto bytes() const noexcept -> ct_packet_data;
	auto bytes() noexcept -> packet_data;

	auto size() const noexcept -> std::size_t;	
	auto data() const noexcept -> const T*;
	auto data() noexcept -> T*;
	auto deleter() const noexcept -> deleter_type;
	auto deleter(deleter_type deleter) noexcept -> deleter_type;

	auto crc32() const noexcept -> std::uint32_t;
	auto split(std::size_t chunk_size) const -> std::vector<packet_buffer>;
private:
	packet_data	m_databits;
	deleter_type m_deleter;	
};
