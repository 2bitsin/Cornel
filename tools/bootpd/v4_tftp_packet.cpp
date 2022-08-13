#include "v4_tftp_packet.hpp"
#include "v4_tftp_consts.hpp"

#include <stdexcept>
#include <type_traits>

auto v4_tftp_packet::error_code_to_string(error_code_type value) noexcept
	-> std::string
{
	using namespace std::string_literals;
	switch (value)
	{
	case undefined:						return "Not defined"s;
	case file_not_found:			return "File not found"s;
	case access_violation:		return "Access violation"s;
	case disk_full:						return "Disk full or allocation exceeded."s;
	case illegal_operation:		return "Illegal TFTP operation."s;
	case unknown_transfer_id: return "Unknown transfer ID."s;
	case file_already_exists: return "File already exists."s;
	case no_such_user:				return "No such user."s;
	default:									return "Unknown error code ("s + std::to_string(value) + ")"s;	
	}		
}

auto v4_tftp_packet::serdes(::serdes<serdes_reader>& _serdes)
	-> ::serdes<serdes_reader>&
{
	using namespace std::string_literals;
	std::uint16_t opcode;
	_serdes(opcode);
	switch (opcode)
	{
	case TFTP_OPCODE_RRQ:
		{
			packet_rrq_type payload_v;
			_serdes(payload_v.path, serdes_asciiz);
			_serdes(payload_v.mode, serdes_asciiz);
			while (!_serdes.empty())
			{
				std::string option, value;
				_serdes(option, serdes_asciiz);
				_serdes(value, serdes_asciiz);
				payload_v.options.emplace(option, value);
			}
			m_value = payload_v;
			break;
		}
	case TFTP_OPCODE_WRQ:
		{
			packet_wrq_type payload_v;
			_serdes(payload_v.path, serdes_asciiz);
			_serdes(payload_v.mode, serdes_asciiz);
			while (!_serdes.empty())
			{
				std::string option, value;
				_serdes(option, serdes_asciiz);
				_serdes(value, serdes_asciiz);
				payload_v.options.emplace(option, value);
			}
			m_value = payload_v;
			break;
		}
	case TFTP_OPCODE_DATA:
		{
			packet_data_type payload_v;
			_serdes(payload_v.block_id);
			while (!_serdes.empty()) {
				std::byte byte_v;
				_serdes(byte_v);
				payload_v.data.emplace_back(byte_v);
			}
			m_value = payload_v;
			break;
		}
	case TFTP_OPCODE_ACK:
		{
			packet_ack_type payload_v;
			_serdes(payload_v.block_id);
			m_value = payload_v;
			break;
		}
	case TFTP_OPCODE_ERROR:
		{
			packet_error_type payload_v;
			_serdes(payload_v.error_code);
			_serdes(payload_v.error_string, serdes_asciiz);
			m_value = payload_v;
			break;
		}
	case TFTP_OPCODE_OACK:
		{
			packet_oack_type payload_v;
			while (!_serdes.empty())
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
	return _serdes;
}

auto v4_tftp_packet::serdes(::serdes<serdes_writer>& _serdes) const ->::serdes<serdes_writer>&
{
	visit([&_serdes]<typename T>(T const& value) 
	{
		if constexpr (std::is_same_v<T, packet_rrq_type>)
		{
			_serdes(TFTP_OPCODE_RRQ);
			_serdes(value.path, serdes_asciiz);
			_serdes(value.mode, serdes_asciiz);
			for (auto const& [option, value] : value.options)
			{
				_serdes(option, serdes_asciiz);
				_serdes(value, serdes_asciiz);
			}
		}
		else if constexpr (std::is_same_v<T, packet_wrq_type>)
		{
			_serdes(TFTP_OPCODE_WRQ);
			_serdes(value.path, serdes_asciiz);
			_serdes(value.mode, serdes_asciiz);
			for (auto const& [option, value] : value.options)
			{
				_serdes(option, serdes_asciiz);
				_serdes(value, serdes_asciiz);
			}
		}
		else if constexpr (std::is_same_v<T, packet_data_type>)
		{
			_serdes(TFTP_OPCODE_DATA);
			_serdes(value.block_id);
			for (auto const& byte : value.data)
			{
				_serdes(byte);
			}
		}
		else if constexpr (std::is_same_v<T, packet_ack_type>)
		{
			_serdes(TFTP_OPCODE_ACK);
			_serdes(value.block_id);
		}
		else if constexpr (std::is_same_v<T, packet_error_type>)
		{
			_serdes(TFTP_OPCODE_ERROR);
			_serdes(value.error_code);
			_serdes(value.error_string, serdes_asciiz);
		}
		else if constexpr (std::is_same_v<T, packet_oack_type>)
		{
			_serdes(TFTP_OPCODE_OACK);
			for (auto const& [option, value] : value.options)
			{
				_serdes(option, serdes_asciiz);
				_serdes(value, serdes_asciiz);
			}			
		}
		else if constexpr (std::is_same_v<T, std::monostate>)
		{
			throw std::logic_error("Can't serialize empty packet.");
		}
		else
		{
			static_assert(!sizeof(T*), "unsupported packet type");
		}
	});
	return _serdes;
}

auto v4_tftp_packet::to_string() const -> std::string
{
	using namespace std::string_literals;
	using namespace std::string_view_literals;

	std::string option_string;

	return std::visit([&]<typename T>(T const& value) 
		-> std::string
	{
		if constexpr (std::is_same_v<T, packet_rrq_type>) {
			for (auto&& [key, val] : value.options) option_string += std::format(", {}=\"{}\"", key, val);
			return std::format("RRQ(file=\"{}\", mode=\"{}\"{})"sv, value.path, value.mode, option_string);
		}
		else if constexpr (std::is_same_v<T, packet_wrq_type>) {
			for (auto&& [key, val] : value.options) option_string += std::format(", {}=\"{}\"", key, val);
			return std::format("WRQ(file=\"{}\", mode=\"{}\"{})"sv, value.path, value.mode, option_string);
		}
		else if constexpr (std::is_same_v<T, packet_data_type>) {
			return std::format("DATA(block_id={}, data=[{} bytes])"sv, value.block_id, value.data.size());
		}
		else if constexpr (std::is_same_v<T, packet_ack_type>) {
			return std::format("ACK(block_id={})"sv, value.block_id);
		}
		else if constexpr (std::is_same_v<T, packet_error_type>) {
			return std::format("ERROR(code={}, message=\"{}\")"sv, error_code_to_string(value.error_code), value.error_string);
		}
		else if constexpr (std::is_same_v<T, packet_oack_type>) {
			for (auto&& [key, val] : value.options) option_string +=  std::format(", {}=\"{}\"", key, val);
			return std::format("OACK(options={})"sv, option_string);
		}
		else if constexpr (std::is_same_v<T, std::monostate>) {
			return "(Nil)"s;			
		}
		else {
			static_assert(!sizeof(T*), "unsupported packet type");
		}
	}, m_value);
}

auto v4_tftp_packet::opcode() const noexcept -> std::uint16_t
{
	return visit([]<typename T>(T const& value) -> std::uint16_t
	{
		if constexpr (std::is_same_v<T, packet_rrq_type>)
			return TFTP_OPCODE_RRQ;
		else if constexpr (std::is_same_v<T, packet_wrq_type>)
			return TFTP_OPCODE_WRQ;
		else if constexpr (std::is_same_v<T, packet_data_type>)
			return TFTP_OPCODE_DATA;
		else if constexpr (std::is_same_v<T, packet_ack_type>)
			return TFTP_OPCODE_ACK;
		else if constexpr (std::is_same_v<T, packet_error_type>)
			return TFTP_OPCODE_ERROR;
		else if constexpr (std::is_same_v<T, packet_oack_type>)
			return TFTP_OPCODE_OACK;
		else if constexpr (std::is_same_v<T, std::monostate>) {
			throw std::logic_error("Can't get opcode of empty packet.");
		}
		else {
			static_assert(!sizeof(T*), "unsupported packet type");
		}
		return 0;
		
	});
}

auto v4_tftp_packet::clear() -> v4_tftp_packet&
{
	m_value = {};
	return *this;
}

auto v4_tftp_packet::set_rrq(std::string_view path, std::string_view mode, dictionary_type options) -> v4_tftp_packet&
{
	m_value = packet_rrq_type(std::string(path), std::string(mode), std::move(options));
	return *this;
}

auto v4_tftp_packet::set_wrq(std::string_view path, std::string_view mode, dictionary_type options) -> v4_tftp_packet&
{
	m_value = packet_wrq_type(std::string(path), std::string(mode), std::move(options));
	return *this;
}

auto v4_tftp_packet::set_data(std::uint16_t block_id, std::span<const std::byte> data) -> v4_tftp_packet&
{
	m_value = packet_data_type(block_id, std::vector<std::byte>(data.begin(), data.end()));
	return *this;
}

auto v4_tftp_packet::set_ack(std::uint16_t block_id) -> v4_tftp_packet&
{
	m_value = packet_ack_type(block_id);
	return *this;
}

auto v4_tftp_packet::set_error(error_code_type error_code, std::string_view error_string) -> v4_tftp_packet&
{
	m_value = packet_error_type(error_code, std::string(error_string));
	return *this;
}

auto v4_tftp_packet::set_oack(dictionary_type options) -> v4_tftp_packet&
{
	m_value = packet_oack_type(std::move(options));
	return *this;
}

auto v4_tftp_packet::serdes_size_hint() const noexcept -> std::size_t
{
	return visit([]<typename T>(T const& value) -> std::size_t 
	{
		if constexpr (std::is_same_v<T, packet_rrq_type> || std::is_same_v<T, packet_wrq_type>)
		{
			std::size_t options_size { 0 };
			for (auto&& [key, val] : value.options) 
				options_size += key.size() + val.size() + 2;
			
			return sizeof(std::uint16_t) + value.path.size() + 1 + value.mode.size() + 1 + options_size;
		}
		else if constexpr (std::is_same_v<T, packet_data_type>)
		{
			return sizeof(std::uint16_t) + sizeof(value.block_id) + value.data.size();
		}
		else if constexpr (std::is_same_v<T, packet_ack_type>)
		{
			return sizeof(std::uint16_t) + sizeof(value.block_id);
		}
		else if constexpr (std::is_same_v<T, packet_error_type>)
		{
			return sizeof(std::uint16_t) + sizeof(value.error_code) + value.error_string.size() + 1;
		}
		else if constexpr (std::is_same_v<T, packet_oack_type>)
		{
			std::size_t options_size { 0 };
			for (auto&& [key, val] : value.options) 
				options_size += key.size() + val.size() + 2;
			return sizeof(std::uint16_t) + options_size;
		}
		else if constexpr (std::is_same_v<T, std::monostate>) {
			throw std::logic_error("Empty packet has no size.");
		}
		else {
			static_assert(!sizeof(T*), "unsupported packet type");
		}
		return 0;
	});
}
