#pragma once

#include <memory_resource>
#include <cstddef>
#include <cstdint>
#include <vector>
#include <deque>
#include <cassert>
#include <optional>

template <typename Allocator = std::pmr::polymorphic_allocator<std::uint8_t>>
struct basic_cobs_decoder
{
	using allocator_type = Allocator;
	using buffer_type = std::vector<uint8_t, allocator_type>;
	using deque_type = std::deque<uint8_t, allocator_type>;

	static inline const constexpr uint8_t COBS_MARK = 0u;
	
	basic_cobs_decoder(allocator_type const& alloc = allocator_type())
	: m_alloc(alloc),
		m_deque(alloc)
	{}
		
	auto init() -> void 
	{
		m_deque.clear();
		m_packets.clear();
	}

	auto done() -> void		
	{
		if (!m_deque.empty()) {
			return decode_packet();		
		}
	}
	
	auto write(const void* data, std::size_t size) -> void
	{				
		for (auto&& value : std::span{ (const uint8_t*)data, size })
		{
			if (value == COBS_MARK) {
				decode_packet();
				continue;
			}			
			m_deque.push_back(value);
		}		
	}

	template <typename T>
	auto write(std::span<T> value) 
		-> std::vector<buffer_type>
	{
		return write(value.data(), value.size()*sizeof(T));
	}

	template <typename T>
	auto write(std::span<const T> value) 		
	{
		return write(value.data(), value.size()*sizeof(T));
	}

	template <typename T>
	auto pop_front(T&& what)
	{
		const auto value = std::move(what.front());
		what.pop_front();
		return value;
	}

	auto decode_packet()		
	{		
		buffer_type packet(m_alloc);
		while(!m_deque.empty())
		{
			const auto len = pop_front(m_deque);
			for (auto i = 1u; i < len; ++i) {
				if (m_deque.empty())
					throw std::runtime_error("cobs_decoder: unexpected end of packet");					
				packet.emplace_back(pop_front(m_deque));				
			}
			if (len < 0xffu && !m_deque.empty()) {
				packet.push_back(COBS_MARK);
			}
		}
		m_packets.emplace_back(std::move(packet));
	}
		
	auto packets() const -> std::vector<buffer_type> const&
	{
		return m_packets;
	}

private:
	allocator_type const& m_alloc;
	deque_type m_deque;
	std::vector<buffer_type> m_packets;
};

using cobs_decoder = basic_cobs_decoder<>;