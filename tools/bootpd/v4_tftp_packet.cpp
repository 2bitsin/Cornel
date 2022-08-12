#include "v4_tftp_packet.hpp"
#include "v4_tftp_consts.hpp"

inline auto v4_tftp_packet::error_code_to_string(error_code_type value) noexcept
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

auto v4_tftp_packet::serdes(::serdes<serdes_reader, network_byte_order>& _serdes)
	-> ::serdes<serdes_reader, network_byte_order>&
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

auto v4_tftp_packet::to_string() const -> std::string
{
	using namespace std::string_literals;
	using namespace std::string_view_literals;

	std::string option_string;

	return std::visit([&]<typename T>(T const& value)
	{
		if constexpr (std::is_same_v<T, packet_rrq_type>) {
			for (auto&& [key, val] : value.options) option_string += (", "s + key + "="s + val);
			return std::format("RRQ(file={}, mode={}{})"sv, value.path, value.mode, option_string);
		}
		else if constexpr (std::is_same_v<T, packet_wrq_type>) {
			for (auto&& [key, val] : value.options) option_string += (", "s + key + "="s + val);
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
			for (auto&& [key, val] : value.options) option_string += (", "s + key + "="s + val);
			return std::format("OACK(options={})"sv, option_string);
		}
		else {
			throw std::logic_error("Unknown packet type."s);
			return ""s;
		}
	}, m_value);
}
