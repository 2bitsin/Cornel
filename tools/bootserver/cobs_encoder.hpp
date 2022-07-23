#pragma once

#include <memory_resource>
#include <cstdint>
#include <cstddef>
#include <cassert>

template <typename Allocator = std::pmr::polymorphic_allocator<std::uint8_t>>
struct basic_cobs_encoder
{
	static inline const constexpr std::uint8_t COBS_MARK = 0u;

	using allocator_type = Allocator;
	
	basic_cobs_encoder(std::size_t pre_allocate = 0, allocator_type const& alloc = allocator_type());

	void write(const void* data, std::size_t size);
		
	auto bytes() const noexcept -> std::span<const uint8_t>;

	template <typename T>
	requires (std::is_trivially_copyable<T>::value)
	auto write(T const&) -> void;

	template <typename T, std::size_t N>
	requires (std::is_trivially_copyable<T>::value)
	auto write(T const (&)[N]) -> void;

	template <typename T>
	requires (std::is_trivially_copyable<T>::value)
	auto write(std::span<const T>) -> void;

	
	auto data() const noexcept -> const uint8_t*;
	auto size() const noexcept -> std::size_t;
	
	auto init() noexcept -> void;
	auto done()	noexcept -> void;
	
private:
	std::vector<uint8_t, allocator_type> m_buffer;
	std::vector<uint8_t, allocator_type> m_ahead;
};

template <typename Allocator>
inline basic_cobs_encoder<Allocator>::basic_cobs_encoder(std::size_t pre_allocate, allocator_type const& alloc)
: m_buffer(alloc),
	m_ahead(alloc)
{
	m_buffer.reserve(pre_allocate);
	m_ahead.reserve(256);
}

template<typename Allocator>
inline void basic_cobs_encoder<Allocator>::write(const void* data, std::size_t size)
{
	
	for (auto&& value : std::span{ (const uint8_t*)data, size })
	{
		if (value != COBS_MARK)
		{			
			m_ahead.push_back(value);
			assert(m_ahead.size() < 0xffu);
			if (m_ahead.size() == 0xfeu)
			{
				m_buffer.push_back(0xffu);
				m_buffer.insert(m_buffer.end(), m_ahead.begin(), m_ahead.end());
				m_ahead.clear();				
			}			
			continue;
		}
		assert(m_ahead.size() < 0xffu);
		m_buffer.push_back((m_ahead.size() + 1u) & 0xffu);
		m_buffer.insert(m_buffer.end(), m_ahead.begin(), m_ahead.end());
		m_ahead.clear();		
	}
}

template<typename Allocator>
inline auto basic_cobs_encoder<Allocator>::bytes() const noexcept -> std::span<const uint8_t>
{
	return{ m_buffer };
}

template<typename Allocator>
inline auto basic_cobs_encoder<Allocator>::data() const noexcept -> const uint8_t*
{
	return bytes().data();
}

template<typename Allocator>
inline auto basic_cobs_encoder<Allocator>::size() const noexcept -> std::size_t
{
	return bytes().size();
}

template<typename Allocator>
inline auto basic_cobs_encoder<Allocator>::init() noexcept -> void
{
	m_buffer.clear();
	m_ahead.clear();
}

template<typename Allocator>
inline auto basic_cobs_encoder<Allocator>::done() noexcept -> void
{
	if (m_ahead.size() > 0)
	{
		assert(m_ahead.size() < 0xffu);
		m_buffer.push_back((m_ahead.size() + 1u) & 0xff);
		m_buffer.insert(m_buffer.end(), m_ahead.begin(), m_ahead.end());
		m_buffer.push_back(COBS_MARK);
		m_ahead.clear();
		
	}
}

template <typename Allocator>
template <typename T>
requires (std::is_trivially_copyable<T>::value)
inline auto basic_cobs_encoder<Allocator>::write(T const& value) -> void
{
	write(&value, sizeof(T));
}
	
template <typename Allocator>
template <typename T, std::size_t N>
requires (std::is_trivially_copyable<T>::value)
inline auto basic_cobs_encoder<Allocator>::write(T const (&value)[N]) -> void
{
	write(&value[0], sizeof(T) * N);
}

template <typename Allocator>
template <typename T>
requires (std::is_trivially_copyable<T>::value)
inline auto basic_cobs_encoder<Allocator>::write(std::span<const T> value) -> void
{
	write(value.data(), sizeof(T)*value.size());
}

using cobs_encoder = basic_cobs_encoder<>;