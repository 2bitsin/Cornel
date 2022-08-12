#pragma once

#include <variant>
#include <unordered_map>
#include <format>

#include <common/serdes.hpp>

struct v4_tftp_packet
{
	struct rrq_type
	{
		std::string path;
		std::string mode;		
	};

	struct wrq_type
	{
		std::string path;
		std::string mode;		
	};

	using payload_type = std::variant<rrq_type, wrq_type>;

	template <auto ... Q>
	auto serdes(serdes<serdes_reader, Q...>& _serdes)
	{
		using namespace std::string_literals;

		std::uint16_t opcode;
		
		_serdes(opcode);
		switch (opcode)
		{
			case 1:
			{
				rrq_type rrq_v;
				_serdes(rrq_v.path, serdes_asciiz);
				_serdes(rrq_v.mode, serdes_asciiz);
				m_value = rrq_v;				
				break;
			}
			case 2:
			{
				wrq_type wrq_v;
				_serdes(wrq_v.path, serdes_asciiz);
				_serdes(wrq_v.mode, serdes_asciiz);
				m_value = wrq_v;				
				break;
			}
		}
		while(!_serdes.empty())
		{
			std::string option, value;
			_serdes(option, serdes_asciiz);
			_serdes(value, serdes_asciiz);
			m_options.emplace(option, value);
		}
	}

	auto to_string() const
	{
		using namespace std::string_literals;
		using namespace std::string_view_literals;
		
		std::string option_string;

		for(auto&& [key, val] : m_options)
			option_string += (", "s+key+"="s+val);

		return std::visit([&]<typename T>(T const& value) 
		{			
			if constexpr (std::is_same_v<T, rrq_type>) {
				return std::format("RRQ(file={}, mode={}{})"sv, value.path, value.mode, option_string);
			} else if constexpr (std::is_same_v<T, wrq_type>) {
				return std::format("WRQ(file={}, mode={}{})"sv, value.path, value.mode, option_string);
			} else {
				return std::format("UNKNOWN({})"sv, option_string);
			}			
		}, m_value);
	}
 
private:
	payload_type m_value;
	std::unordered_map<std::string, std::string> m_options;
};