#pragma once

#include <array>
#include <memory>
#include <span>
#include <tuple>
#include <utility>

#include <common/serdes.hpp>

struct v4_dhcp_options
{
	template <byte_order_type Byte_order>
	auto serdes(::serdes<serdes_reader, Byte_order>& _serdes) 
		-> ::serdes<serdes_reader, Byte_order>&
	{
		using std::make_unique;
		using std::unique_ptr;
		using std::uint8_t;
		using std::span;

		unique_ptr<uint8_t[]> data;
		uint8_t code;
		uint8_t size;
		
		while(true)
		{
			_serdes(code);
			if (code == 0x00u) continue;
			if (code == 0xffu) break;
			_serdes(size);
			data = make_unique<uint8_t[]>(size + 1u);
			data[0] = size;
			_serdes(span{ data.get() + 1u, size });
			m_values[code - 1u] = std::move (data);
		}		
		return _serdes;
	}

	template <byte_order_type Byte_order>
	auto serdes(::serdes<serdes_writter, Byte_order>& _serdes) 
		-> ::serdes<serdes_writter, Byte_order>&
	{
		using std::make_unique;
		using std::unique_ptr;
		using std::uint8_t;
		using std::span;
		
		const auto count_options = std::min(std::size(m_values), 254u);
		for (auto i = 0u; i < count_options; ++i)
		{
			if (m_values[i] == nullptr)
				continue;
			_serdes(std::uint8_t(i + 1u));
			_serdes(span{ m_values[i].get(), m_values[i].get()[0] + 1u});
		}	
		return _serdes;
	}
	
	auto operator[] (std::uint8_t index) 
		-> std::span<const std::uint8_t>
	{
		if (m_values [index] == nullptr) 
		{
			return {};
		}
		
		return { m_values [index].get() + 1u, 
			       m_values [index][0] };
	}

	auto set(std::uint8_t code, std::span<const std::uint8_t> data)
		-> bool
	{
		using std::make_unique;
		using std::uint8_t;
		if (code < 1u || code > 254u || data.size() > 255u)
			return false;
		m_values[code - 1u] = make_unique<uint8_t[]>(data.size() + 1u);
		m_values[code - 1u][0] = (std::uint8_t)data.size();
		std::copy(data.begin(), data.end(), m_values[code - 1u].get() + 1u);
	}
	
	template <typename... Q>
	auto set(std::uint8_t code, Q&&... args)
		-> bool
	{
		using std::make_unique;
		using std::uint8_t;

		static constexpr auto total_size = (sizeof(Q) + ... + 0);
		if (total_size > 255u)
			return false;
		
		m_values[code - 1u] = make_unique<uint8_t[]>(total_size + 1u);
		m_values[code - 1u][0] = (uint8_t)total_size;		
		::serdes<serdes_writter, network_byte_order> _serdes (
			std::span(m_values[code - 1u].get() + 1u, total_size)
		);
		(_serdes(std::forward<Q>(args)), ...);
		return true;
	}

	template <typename... Q>
	auto get(std::uint8_t code, std::tuple<Q...>& values)
		-> bool
	{
		return get(code, values, std::make_index_sequence<sizeof...(Q)>());	
	}

	template <typename... Q>
	auto get(std::uint8_t code, std::tuple<Q&...> values)
		-> bool
	{
		return get(code, values, std::make_index_sequence<sizeof...(Q)>());	
	}

	auto assign(std::uint8_t code, v4_dhcp_options const& from)
	{
		using std::make_unique;
		if (from.m_values[code - 1u] == nullptr)
			return false;
		auto& src_value = from.m_values[code - 1u];
		auto& dst_value = m_values[code - 1u];
		dst_value = make_unique<uint8_t[]>(src_value[0] + 1u);
		std::copy(src_value.get(), src_value.get() + src_value[0] + 1u, dst_value.get());
		return true;
	}
	
	
protected:
	template <typename Tuple, std::size_t ... Index>
	auto get(std::uint8_t code, Tuple& values, std::index_sequence<Index...>)
		-> bool
	{
		if (code < 1u || code > 254u || !m_values[code - 1u])
			return false;
		
		::serdes<serdes_reader, network_byte_order> _serdes(
			std::span(m_values[code - 1u].get() + 1u, m_values[code - 1u][0])
		);

		((_serdes(std::get<Index>(values))),...);
		return true;
	}

private:
	std::unique_ptr<std::uint8_t[]> m_values[254u];
};