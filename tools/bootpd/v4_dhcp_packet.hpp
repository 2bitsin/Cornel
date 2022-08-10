#pragma once

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <span>
#include <optional>

#include <common/serdes.hpp>

#include "v4_dhcp_consts.hpp"
#include "v4_dhcp_options.hpp"

struct v4_dhcp_packet
{
	using mac_address_type = std::uint8_t[6];	
	v4_dhcp_packet()
	{}

	v4_dhcp_packet(std::span<const std::uint8_t> bits)
	{
		::serdes<serdes_reader, network_byte_order> _serdes (bits);
		_serdes(*this);
	}

	template <auto... U>
	v4_dhcp_packet(serdes<U...>& _serdes)
	{
		_serdes(*this);
	}

	template <typename _Serdes>
	auto serdes(_Serdes& _serdes) -> _Serdes&
	{
		SERDES_APPLY(_serdes, m_opcode);
		SERDES_APPLY(_serdes, m_hardware_type);
		SERDES_APPLY(_serdes, m_hardware_address_length);
		SERDES_APPLY(_serdes, m_number_of_hops);
		SERDES_APPLY(_serdes, m_transaction_id);
		SERDES_APPLY(_serdes, m_seconds_elapsed);
		SERDES_APPLY(_serdes, m_flags);
		SERDES_APPLY(_serdes, m_client_ip_address_v4);
		SERDES_APPLY(_serdes, m_your_ip_address_v4);
		SERDES_APPLY(_serdes, m_server_ip_address_v4);
		SERDES_APPLY(_serdes, m_gateway_ip_address_v4);
		SERDES_APPLY(_serdes, m_client_hardware_address);
		SERDES_APPLY(_serdes, m_server_host_name);
		SERDES_APPLY(_serdes, m_boot_file_name);		
		SERDES_APPLY(_serdes, m_options);
		return _serdes;
	}

	auto pretty_print(std::ostream& oss) const -> std::ostream&;
	
	friend inline auto operator<<(std::ostream& oss, v4_dhcp_packet const& pkt) -> std::ostream&
	{
		return pkt.pretty_print(oss);
	}

	auto is_message_type(std::uint8_t msg_type) const -> bool		
	{
		if (auto mto = m_options.message_type(); mto.has_value())
			return mto.value() == msg_type;
		return false;
	}
	
	auto requested_parameters() const
		-> std::span<const std::uint8_t>
	{
		return m_options.requested_parameters();
	}

	bool is_request  () const { return m_opcode == DHCP_OPCODE_REQUEST; }
	bool is_response () const { return m_opcode == DHCP_OPCODE_RESPONSE; }

	auto options() -> v4_dhcp_options&
	{
		return m_options;
	}
	auto options() const -> v4_dhcp_options const&
	{
		return m_options;
	}
	
	auto assign_options(v4_dhcp_options const& from, std::span<const std::uint8_t> which)
	{
		for(auto&& index : which) 
		{
			m_options.assign(index, from);
		}
	}	
	
	auto opcode() const -> std::uint8_t
	{
		return m_opcode;
	}
	auto hardware_type() const -> std::uint8_t
	{
		return m_hardware_type;	
	}	
	auto hardware_address_length () const -> std::uint8_t
	{
		return m_hardware_address_length;
	}	
	auto hardware_address() -> std::span<const std::uint8_t>
	{
		return  { m_client_hardware_address, m_hardware_address_length };
	}
	auto client_address () const -> std::uint32_t
	{
		return m_client_ip_address_v4;
	}	
	auto your_address () const -> std::uint32_t
	{
		return m_your_ip_address_v4;
	}
	auto server_address () const -> std::uint32_t
	{
		return m_server_ip_address_v4;
	}
	auto gateway_address () const -> std::uint32_t
	{
		return m_gateway_ip_address_v4;
	}
	auto transaction_id () const -> std::uint32_t
	{
		return m_transaction_id;
	}	
	auto seconds_elapsed () const -> std::uint16_t
	{
		return m_seconds_elapsed;
	}
	auto flags () const -> std::uint16_t
	{
		return m_flags;
	}
	auto host_name () const -> std::string_view
	{
		return { m_server_host_name, strnlen_s(m_server_host_name, sizeof(m_server_host_name)) };
	}
	auto boot_file_name () const -> std::string_view
	{
		return { m_boot_file_name, strnlen_s(m_boot_file_name, sizeof(m_boot_file_name)) };
	}

	auto opcode(std::uint8_t value) 
	{ m_opcode = value; }
	auto hardware_type(std::uint8_t value) 
	{ m_hardware_type = value; }
	auto hardware_address(std::span<const std::uint8_t> value) 
	{
		if (value.size() > sizeof(m_client_hardware_address))
			throw std::logic_error("hardware_address too large");
		m_hardware_address_length = value.size();
		std::copy(value.begin(), value.end(), m_client_hardware_address);
	}
	auto client_address(std::uint32_t value) 
	{ m_client_ip_address_v4 = value; }
	auto your_address(std::uint32_t value) 
	{ m_your_ip_address_v4 = value; }
	auto server_address(std::uint32_t value) 
	{ m_server_ip_address_v4 = value; }
	auto gateway_address(std::uint32_t value) 
	{ m_gateway_ip_address_v4 = value; }
	auto transaction_id(std::uint32_t value) 
	{ m_transaction_id = value; }
	auto seconds_elapsed(std::uint16_t value) 
	{ m_seconds_elapsed = value; }
	auto flags(std::uint16_t value) 
	{ m_flags = value; }
	auto host_name(std::string_view value) 
	{
		if (value.size() > sizeof(m_server_host_name))
			throw std::logic_error("host_name too large");
		std::fill(std::begin(m_server_host_name), std::end(m_server_host_name), 0);
		std::copy(value.begin(), value.end(), m_server_host_name);
	}
	auto boot_file_name(std::string_view value) 
	{
		if (value.size() > sizeof(m_boot_file_name))
			throw std::logic_error("boot_file_name too large");
		std::fill(std::begin(m_boot_file_name), std::end(m_boot_file_name), 0);
		std::copy(value.begin(), value.end(), m_boot_file_name);
	}
	

protected:

	// Primary
	std::uint8_t		m_opcode;
	std::uint8_t		m_hardware_type;
	std::uint8_t		m_hardware_address_length;
	std::uint8_t		m_number_of_hops;
	std::uint32_t		m_transaction_id;
	std::uint16_t		m_seconds_elapsed;
	std::uint16_t		m_flags;
	std::uint32_t		m_client_ip_address_v4;
	std::uint32_t		m_your_ip_address_v4;
	std::uint32_t		m_server_ip_address_v4;
	std::uint32_t		m_gateway_ip_address_v4;
	std::uint8_t		m_client_hardware_address [16];
	char						m_server_host_name [64];
	char						m_boot_file_name [128];	
	v4_dhcp_options m_options;
};

