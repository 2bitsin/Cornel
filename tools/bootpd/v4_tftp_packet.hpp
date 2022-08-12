#pragma once

#include <variant>
#include <unordered_map>
#include <format>

#include <common/serdes.hpp>

struct v4_tftp_packet
{
	enum error_code_type : std::uint16_t
	{
		undefined,
		file_not_found,
		access_violation,
		disk_full,
		illegal_operation,
		unknown_transfer_id,
		file_already_exists,
		no_such_user		
	};

	static auto error_code_to_string(error_code_type value) noexcept
	{
		switch (value)
		{
		case undefined:						return "undefined";
		case file_not_found:			return "file_not_found";
		case access_violation:		return "access_violation";
		case disk_full:						return "disk_full";
		case illegal_operation:		return "illegal_operation";
		case unknown_transfer_id: return "unknown_transfer_id";
		case file_already_exists: return "file_already_exists";
		case no_such_user:				return "no_such_user";
		default:									return "unknown";
		}
	}

	struct packet_rrq_type
	{
		std::string path;
		std::string mode;	
		std::unordered_map<std::string, std::string> options;

	};

	struct packet_wrq_type
	{
		std::string path;
		std::string mode;		
		std::unordered_map<std::string, std::string> options;
	};

	struct packet_data_type
	{
		std::uint16_t block_id;
		std::vector<std::byte> data;		
	};

	struct packet_ack_type
	{
		std::uint16_t block_id;
	};

	struct packet_error_type
	{
		error_code_type error_code;
		std::string error_string;
	};

	struct packet_oack_type
	{
		std::unordered_map<std::string, std::string> options;
	};

	using payload_type = std::variant<packet_rrq_type, packet_wrq_type, packet_data_type, packet_ack_type, packet_error_type, packet_oack_type>;

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
				packet_rrq_type payload_v;
				_serdes(payload_v.path, serdes_asciiz);
				_serdes(payload_v.mode, serdes_asciiz);
				while(!_serdes.empty())
				{
					std::string option, value;
					_serdes(option, serdes_asciiz);
					_serdes(value, serdes_asciiz);
					payload_v.options.emplace(option, value);
				}
				m_value = payload_v;				
				break;
			}
			case 2:
			{
				packet_wrq_type payload_v;
				_serdes(payload_v.path, serdes_asciiz);
				_serdes(payload_v.mode, serdes_asciiz);
				while(!_serdes.empty())
				{
					std::string option, value;
					_serdes(option, serdes_asciiz);
					_serdes(value, serdes_asciiz);
					payload_v.options.emplace(option, value);
				}
				m_value = payload_v;
				break;
			}
			case 3:
			{
				packet_data_type payload_v;
				_serdes(payload_v.block_id);
				while(!_serdes.empty()) {
					std::byte byte_v;
					_serdes(byte_v);
					payload_v.data.emplace_back(byte_v);
				}				
				m_value = payload_v;
				break;
			}
			case 4:
			{
				packet_ack_type payload_v;
				_serdes(payload_v.block_id);
				m_value = payload_v;
				break;
			}
			case 5:
			{
				packet_error_type payload_v;
				_serdes(payload_v.error_code);
				_serdes(payload_v.error_string, serdes_asciiz);
				m_value = payload_v;
				break;
			}
			case 6:
			{
				packet_oack_type payload_v;
				while(!_serdes.empty())
				{
					std::string option, value;
					_serdes(option, serdes_asciiz);
					_serdes(value, serdes_asciiz);
					payload_v.options.emplace(option, value);
				}
				m_value = payload_v;
				break;
			}
		}
	}

	auto to_string() const
	{
		using namespace std::string_literals;
		using namespace std::string_view_literals;
		
		std::string option_string;

		return std::visit([&]<typename T>(T const& value) 
		{			
			if constexpr (std::is_same_v<T, packet_rrq_type>) {
				for(auto&& [key, val] : value.options) option_string += (", "s+key+"="s+val);
				return std::format("RRQ(file={}, mode={}{})"sv, value.path, value.mode, option_string);
			} 
			else if constexpr (std::is_same_v<T, packet_wrq_type>) {
				for(auto&& [key, val] : value.options) option_string += (", "s+key+"="s+val);
				return std::format("WRQ(file={}, mode={}{})"sv, value.path, value.mode, option_string);
			} 
			else if constexpr (std::is_same_v<T, packet_data_type>) {
				return std::format("DATA(block_id={}, data={} bytes)"sv, value.block_id, value.data.size());
			} 
			else if constexpr (std::is_same_v<T, packet_ack_type>) {
				return std::format("ACK(block_id={})"sv, value.block_id);
			} 
			else if constexpr (std::is_same_v<T, packet_error_type>) {
				return std::format("ERROR(code={}, message={})"sv, error_code_to_string(value.error_code), value.error_string);
			} 
			else if constexpr (std::is_same_v<T, packet_oack_type>) {
				for(auto&& [key, val] : value.options) option_string += (", "s+key+"="s+val);
				return std::format("OACK(options={})"sv, option_string);
			} 
			else {
				throw std::logic_error("Unknown packet type."s);
			}
		}, m_value);
	}
 
private:
	payload_type m_value;
};