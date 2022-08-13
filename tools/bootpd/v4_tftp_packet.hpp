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

	static auto error_code_to_string(error_code_type value) noexcept -> std::string; 

	using dictionary_type = std::unordered_map<std::string, std::string>;

	struct packet_rrq_type
	{
		std::string path;
		std::string mode;	
		dictionary_type options;		
	};	

	struct packet_wrq_type
	{
		std::string path;
		std::string mode;	
		dictionary_type options;		
	};

	struct packet_data_type
	{
		std::uint16_t block_id;
		std::vector<std::byte> data;
	};

	struct packet_ack_type
	{ std::uint16_t block_id; };

	struct packet_error_type
	{
		error_code_type error_code;
		std::string error_string;
	};

	struct packet_oack_type
	{
		 dictionary_type options;
	};

	using payload_type = std::variant<std::monostate, packet_rrq_type, packet_wrq_type, packet_data_type, packet_ack_type, packet_error_type, packet_oack_type>;

	auto serdes(::serdes<serdes_reader>& _serdes)  -> ::serdes<serdes_reader>&;	
	auto serdes(::serdes<serdes_writer>& _serdes) const -> ::serdes<serdes_writer>&;	

	auto to_string() const -> std::string;

	template <typename Func>
	auto visit(Func&& func) const
	{ return std::visit(std::forward<Func>(func), m_value); }
	
	auto opcode() const noexcept -> std::uint16_t;

	auto clear() -> v4_tftp_packet&;
	auto set_rrq(std::string_view path, std::string_view mode, dictionary_type options) -> v4_tftp_packet&;
	auto set_wrq(std::string_view path, std::string_view mode, dictionary_type options) -> v4_tftp_packet&;
	auto set_data(std::uint16_t block_id, std::span<const std::byte> data) -> v4_tftp_packet&;
	auto set_ack(std::uint16_t block_id) -> v4_tftp_packet&;
	auto set_error(error_code_type error_code, std::string_view error_string) -> v4_tftp_packet&;
	auto set_oack(dictionary_type options) -> v4_tftp_packet&;

	auto serdes_size_hint() const noexcept -> std::size_t;

private:
	payload_type m_value;
};