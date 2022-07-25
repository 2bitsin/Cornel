#pragma once

#include <memory_resource>
#include <cstddef>
#include <cstdint>
#include <vector>
#include <deque>
#include <cassert>
#include <optional>


struct cobs_decoder
{	
	using buffer_type = std::vector<uint8_t>;
	using deque_type = std::deque<uint8_t>;

	static inline const constexpr uint8_t COBS_MARK = 0u;
	
	cobs_decoder()	
	{}
		
	auto init() -> void 
	{
		m_deque.clear();
		m_packets.clear();
	}

	auto done() -> void		
	{
		if (!m_deque.empty()) {
			decode_packet();		
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
	auto write(std::span<T> value) -> void		
	{
		return write(value.data(), value.size()*sizeof(T));
	}

	template <typename T>
	auto write(std::span<const T> value) -> void
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

	auto decode_packet() -> void
	{		
		buffer_type packet;
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
	deque_type m_deque;
	std::vector<buffer_type> m_packets;
};
