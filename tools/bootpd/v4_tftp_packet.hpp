#pragma once

#include <variant>
#include <unordered_map>
#include <format>
#include <string>
#include <string_view>

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

	static auto error_code_to_string(error_code_type value) noexcept;

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

	using payload_type = std::variant<std::monostate 
		                               ,packet_rrq_type 
		                               ,packet_wrq_type 
		                               ,packet_data_type 
		                               ,packet_ack_type
		                               ,packet_error_type 
		                               ,packet_oack_type>;

	auto serdes(::serdes<serdes_reader, network_byte_order>& _serdes) 
		-> ::serdes<serdes_reader, network_byte_order>&;

	auto to_string() const -> std::string;

	template <typename Func>
	auto visit(Func&& func) const
	{
		std::visit(std::forward<Func>(func), m_value);
	}
 
private:
	payload_type m_value;
};