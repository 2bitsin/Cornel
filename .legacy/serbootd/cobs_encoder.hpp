#pragma once

#include <memory_resource>
#include <cstdint>
#include <cstddef>
#include <vector>
#include <cassert>


struct cobs_encoder
{
	static inline const constexpr std::uint8_t COBS_MARK = 0u;
		
	cobs_encoder(std::size_t pre_allocate = 0);

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

	template <typename T>
	requires (std::is_trivially_copyable<T>::value)
	auto write(std::span<T>) -> void;

	
	auto data() const noexcept -> const uint8_t*;
	auto size() const noexcept -> std::size_t;
	
	auto init() noexcept -> void;
	auto done()	noexcept -> void;
	
private:
	std::vector<uint8_t> m_buffer;
	std::vector<uint8_t> m_ahead;
};


inline cobs_encoder::cobs_encoder(std::size_t pre_allocate)
{
	m_buffer.reserve(pre_allocate);
	m_ahead.reserve(256);
}

inline void cobs_encoder::write(const void* data, std::size_t size)
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

inline auto cobs_encoder::bytes() const noexcept -> std::span<const uint8_t>
{
	return{ m_buffer };
}

inline auto cobs_encoder::data() const noexcept -> const uint8_t*
{
	return bytes().data();
}

inline auto cobs_encoder::size() const noexcept -> std::size_t
{
	return bytes().size();
}

inline auto cobs_encoder::init() noexcept -> void
{
	m_buffer.clear();
	m_ahead.clear();
}

inline auto cobs_encoder::done() noexcept -> void
{
	assert(m_ahead.size() < 0xffu);
	m_buffer.push_back((m_ahead.size() + 1u) & 0xff);
	if (m_ahead.size() > 0)
		m_buffer.insert(m_buffer.end(), m_ahead.begin(), m_ahead.end());
	m_buffer.push_back(COBS_MARK);		
	m_ahead.clear();	
}

template <typename T>
requires (std::is_trivially_copyable<T>::value)
inline auto cobs_encoder::write(T const& value) -> void
{
	write(&value, sizeof(T));
}
	
template <typename T, std::size_t N>
requires (std::is_trivially_copyable<T>::value)
inline auto cobs_encoder::write(T const (&value)[N]) -> void
{
	write(&value[0], sizeof(T) * N);
}

template <typename T>
requires (std::is_trivially_copyable<T>::value)
inline auto cobs_encoder::write(std::span<const T> value) -> void
{
	write(value.data(), sizeof(T)*value.size());
}

template <typename T>
requires (std::is_trivially_copyable<T>::value)
inline auto cobs_encoder::write(std::span<T> value) -> void
{
	write(value.data(), sizeof(T)*value.size());
}
