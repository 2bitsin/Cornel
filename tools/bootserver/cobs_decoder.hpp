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
	}

	template <typename Output>
	auto done(Output&& frames) -> void		
	{
		if (!m_deque.empty()) {
			return decode_packet(frames);		
		}
	}
	
	template <typename Output>
	auto write(const void* data, std::size_t size, Output&& frames) -> void
	{				
		for (auto&& value : std::span{ (const uint8_t*)data, size })
		{
			if (value != COBS_MARK)
			{
				m_deque.push_back(value);
				continue;
			}
			decode_packet(frames);
		}		
	}

	template <typename T, typename Output>
	auto write(std::span<T> value, Output&& frames) 
		-> std::vector<buffer_type>
	{
		return write(value.data(), value.size()*sizeof(T), frames);
	}

	template <typename T, typename Output>
	auto write(std::span<const T> value, Output&& frames) 		
	{
		return write(value.data(), value.size()*sizeof(T), frames);
	}

	template <typename Output>
	auto decode_packet(Output&& frames) 
		
	{		
		buffer_type packet(m_alloc);
		while(!m_deque.empty())
		{
			auto length = m_deque.front();
			m_deque.pop_front();
			for (auto i = 1u; i < length; ++i)
			{
				if (m_deque.empty())
					throw std::runtime_error("cobs_decoder: unexpected end of packet");					
				packet.push_back(m_deque.front());
				m_deque.pop_front();
			}
			if (length < 0xffu)
				packet.push_back(COBS_MARK);
		}
		frames.emplace_back(std::move(packet));
	}
		
private:
	allocator_type const& m_alloc;
	deque_type m_deque;
};

using cobs_decoder = basic_cobs_decoder<>;