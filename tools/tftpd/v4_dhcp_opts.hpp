#pragma once

#include <variant>
#include <unordered_map>
#include <cstdint>
#include <cstddef>
#include <memory>
#include <string>

#include "common/serdes.hpp"

struct v4_dhcp_opts	
{
	static inline const constexpr std::uint8_t CODE_PAD = 0;
	static inline const constexpr std::uint8_t CODE_END = 255;
	static inline const constexpr std::uint8_t CODE_MESSAGE_TYPE = 53;
	static inline const constexpr std::uint8_t CODE_SERVER_IDENTIFIER = 54;
	static inline const constexpr std::uint8_t CODE_PARAMETER_REQUEST_LIST = 55;
	static inline const constexpr std::uint8_t CODE_MAXIMUM_MESSAGE_SIZE = 57;
	static inline const constexpr std::uint8_t CODE_GUID_BASED_CLASS_IDENTIFIER = 97;
	static inline const constexpr std::uint8_t CODE_VENDOR_CLASS_IDENTIFIER = 60;
	static inline const constexpr std::uint8_t CODE_CLIEANT_SYSTEM_ARCHITECTURE = 93;
	static inline const constexpr std::uint8_t CODE_CLIENT_NETWORK_DEVICE_INTERFACE = 94;	

	struct option_generic
	{
		template <auto...Q> option_generic(serdes<Q...>& _serdes) { _serdes.deserialize(*this); }		
		std::uint8_t length;
		std::unique_ptr<std::uint8_t [] > value;
		auto& deserialize(auto& _serdes)
		{
			_serdes.deserialize(length);
			value = std::make_unique<std::uint8_t []>(length);
			_serdes.deserialize(std::span(value.get(), length));
			return _serdes;
		}
	};
	
	template <uint8_t code>
	struct option;

	template <>
	struct option<CODE_MESSAGE_TYPE>
	{
		template <auto...Q> option(serdes<Q...>& _serdes) { _serdes.deserialize(*this); }
		std::uint8_t length;
		std::uint8_t value;
		auto& deserialize(auto& _serdes)
		{
			_serdes.deserialize(length);
			_serdes.deserialize(value);
			return _serdes;
		}
	};

	template <>
	struct option<CODE_PARAMETER_REQUEST_LIST> 
	{
		template <auto...Q> option(serdes<Q...>& _serdes) { _serdes.deserialize(*this); }
		uint8_t length;
		std::unique_ptr<uint8_t []> values;
		auto& deserialize(auto& _serdes)
		{
			_serdes.deserialize(length);
			values = std::make_unique<uint8_t []>(length);
			_serdes.deserialize(std::span(values.get(), length));
			return _serdes;
		}
	};		

	template <>
	struct option<CODE_MAXIMUM_MESSAGE_SIZE>  
	{
		template <auto...Q> option(serdes<Q...>& _serdes) { _serdes.deserialize(*this); }
		uint8_t length;
		uint16_t value;		
		auto& deserialize(auto& _serdes)
		{
			_serdes.deserialize(length);
			_serdes.deserialize(value);
			return _serdes;
		}
	};

	template <>
	struct option<CODE_GUID_BASED_CLASS_IDENTIFIER>
	{
		template <auto...Q> option(serdes<Q...>& _serdes) { _serdes.deserialize(*this); }
		uint8_t length;
		uint8_t type;
		std::unique_ptr<uint8_t []> value;
		auto& deserialize(auto& _serdes)
		{
			_serdes.deserialize(length);
			_serdes.deserialize(type);
			value = std::make_unique<uint8_t []>(length - 1);
			_serdes.deserialize(std::span(value.get(), length - 1));
			return _serdes;
		}
	};
	
	template <>
	struct option<CODE_CLIEANT_SYSTEM_ARCHITECTURE> 
	{
		template <auto...Q> option(serdes<Q...>& _serdes) { _serdes.deserialize(*this); }
		uint8_t length;
		uint16_t value;
		auto& deserialize(auto& _serdes)
		{
			_serdes.deserialize(length);
			_serdes.deserialize(value);
			return _serdes;
		}
	};

	template <>
	struct option<CODE_CLIENT_NETWORK_DEVICE_INTERFACE>
	{
		template <auto...Q> option(serdes<Q...>& _serdes) { _serdes.deserialize(*this); }
		uint8_t length;
		uint8_t type;
		uint8_t version_major;
		uint8_t version_minor;
		auto& deserialize(auto& _serdes)
		{
			_serdes.deserialize(length);
			_serdes.deserialize(type);
			_serdes.deserialize(version_major);
			_serdes.deserialize(version_minor);
			return _serdes;
		}
	};

	template <>
	struct option<CODE_VENDOR_CLASS_IDENTIFIER>
	{
		template <auto...Q> option(serdes<Q...>& _serdes) { _serdes.deserialize(*this); }
		uint8_t length;
		std::unique_ptr<char []> value;
		auto& deserialize(auto& _serdes)
		{
			_serdes.deserialize(length);			
			value = std::make_unique<char []>(length);
			_serdes.deserialize(std::span(value.get(), length));			
			return _serdes;
		}
	};

	using option_var = std::variant<
		option_generic,
		option<CODE_MESSAGE_TYPE>,
		option<CODE_PARAMETER_REQUEST_LIST>,
		option<CODE_MAXIMUM_MESSAGE_SIZE>,
		option<CODE_GUID_BASED_CLASS_IDENTIFIER>,
		option<CODE_CLIEANT_SYSTEM_ARCHITECTURE>,
		option<CODE_CLIENT_NETWORK_DEVICE_INTERFACE>,
		option<CODE_VENDOR_CLASS_IDENTIFIER>
	>;		

	template<typename _Serdes>
	auto deserialize(_Serdes& _serdes) 
		-> _Serdes&
	{
		
		uint8_t code;
		std::unordered_multimap<std::uint8_t, option_var> list;
		while(true)
		{
			_serdes.deserialize(code);			
			if (code == CODE_END)
				break;
			switch (code)
			{
      case CODE_PAD:															continue;			
      case CODE_MESSAGE_TYPE:                     list.emplace(code, option<CODE_MESSAGE_TYPE                    >(_serdes)); break;
      case CODE_PARAMETER_REQUEST_LIST:           list.emplace(code, option<CODE_PARAMETER_REQUEST_LIST          >(_serdes)); break;
      case CODE_MAXIMUM_MESSAGE_SIZE:             list.emplace(code, option<CODE_MAXIMUM_MESSAGE_SIZE            >(_serdes)); break;
			case CODE_GUID_BASED_CLASS_IDENTIFIER:      list.emplace(code, option<CODE_GUID_BASED_CLASS_IDENTIFIER     >(_serdes)); break;
			case CODE_CLIEANT_SYSTEM_ARCHITECTURE:      list.emplace(code, option<CODE_CLIEANT_SYSTEM_ARCHITECTURE     >(_serdes)); break;
      case CODE_CLIENT_NETWORK_DEVICE_INTERFACE:  list.emplace(code, option<CODE_CLIENT_NETWORK_DEVICE_INTERFACE >(_serdes)); break;
      case CODE_VENDOR_CLASS_IDENTIFIER:          list.emplace(code, option<CODE_VENDOR_CLASS_IDENTIFIER         >(_serdes)); break;
      default:                                    list.emplace(code, option_generic(_serdes)); break;
      }

		}	
		m_list = std::move(list);
		return _serdes;
	}
	
	std::unordered_multimap<std::uint8_t, option_var> m_list;	
};