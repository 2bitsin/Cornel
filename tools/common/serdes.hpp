#pragma once

#include "byte_order.hpp"

#include <vector>
#include <string>
#include <cstdint>
#include <span>

enum byte_order_type : bool
{
	network_byte_order = true,
	host_byte_order = false
};

enum is_required_type : bool
{
	required = true,
	optional = false
};

enum array_subtype: uint8_t
{
	zero_terminated,
	length_prefixed
};

enum serdes_type : uint8_t
{
	serdes_reader,
	serdes_writter
};

template <serdes_type Serdes_type, byte_order_type Byte_order = network_byte_order>
struct serdes;

template <byte_order_type Byte_order>
struct serdes<serdes_reader, Byte_order>
{
	template <typename T>
	requires (std::is_trivial_v<T> && sizeof(T) == 1u)
	serdes(std::span<const T> bits)
	:	m_data{ (const std::byte*)bits.data(), bits.size() },
		m_curr{ (const std::byte*)bits.data(), bits.size() }
	{}

	template <typename T>
	requires requires (T& value) 
	{ value.deserialize(std::declval<serdes&>()); }
	auto& deserialize(T& value)
	{
		value.deserialize(*this);
		return *this;
	}

	template <typename T>
	requires (std::is_triavial_v<T>)
	auto& deserialize(T& value) 
	{
		if (m_curr.size() < sizeof(T)) {
			throw std::runtime_error("deserialize: not enough data");
			return *this;
		}
		
		value = *(const T*)(m_curr.data());
		if constexpr (Byte_order == network_byte_order)
			net_to_host_inplace(value);
		m_curr = m_curr.subspan(sizeof(T));
		return *this;
	}

	template <typename T, std::size_t N>
	auto& deserialize(T (&value)[N])
	{
		for (std::size_t i = 0; i < N; ++i)
			deserialize (value[i]);
		return *this;
	}
		
	template <array_subtype Array_subtype = zero_terminated, typename Array_container>
	requires (!std::is_trivial_v<Array_container>)
	auto& desrialize_array_to_container(Array_container& output_value)
	{
		using value_type = typename Array_container::value_type;

		value_type char_value;
		Array_container value;
		
		if constexpr (Array_subtype == zero_terminated)
		{
		L_next_char:
			deserialize (char_value);
			if (char_value == value_type(0)) {
				output_value = std::move(value);
				return *this;
			}
			value.push_back(char_value);
			goto L_next_char;			
		}
		
		if constexpr (Array_subtype == length_prefixed)
		{
			deserialize (char_value);
			auto remaining_chars = (std::size_t)char_value;
		L_next_char:
			deserialize (char_value);
			value.push_back(char_value);
			--remaining_chars;
			if (remaining_chars > 0)
				goto L_next_char;		
			output_value = std::move(value);
			return *this;
		}		
	}
	
	template <array_subtype Array_subtype, typename T>
	auto& deserialize(std::vector<T>& output_value)
	{
		return deserialize_array_to_cotainer<Array_subtype>(output_value);
	}

	template <array_subtype Array_subtype, typename T>
	auto& deserialize(std::basic_string<T>& output_value)
	{
		return deserialize_array_to_cotainer<Array_subtype>(output_value);
	}

	template <typename T>
	auto& deserialize(std::span<T>& output_value)
	{
		for(auto&& value : output_value) {
			deserialize(value);
		}	
	}
	
	auto& skip(std::size_t number_of_bytes)
	{
		if (m_curr.size() < number_of_bytes) {
			throw std::runtime_error("skipping past end of data");
			return *this;
		}		
		m_curr = m_curr.subspan(number_of_bytes);
		return *this;		
	}
	
	auto& seek(std::size_t byte_offset)
	{
		if (m_data.size() < byte_offset) {
			throw std::runtime_error("seeking pas end of data");
			return *this;
		}		
		m_curr = m_data.subspan(byte_offset);
		return *this;		
	}
	
	auto& rewind()
	{
		return seek(0u);
	}

private:
	std::span<const std::byte> m_curr;
	std::span<const std::byte> m_data;
};