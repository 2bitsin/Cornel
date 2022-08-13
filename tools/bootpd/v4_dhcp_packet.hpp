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
	v4_dhcp_packet();
	v4_dhcp_packet(std::span<const std::uint8_t> bits);	
	v4_dhcp_packet(::serdes<serdes_reader>& _serdes);
	
	auto serdes(::serdes<serdes_writer>& _serdes) const -> ::serdes<serdes_writer>&;		
	auto serdes(::serdes<serdes_reader>& _serdes) -> ::serdes<serdes_reader>&;
	auto pretty_print(std::ostream& oss) const -> std::ostream&;	
	
	auto options()->v4_dhcp_options&;
	auto options() const->v4_dhcp_options const&;	
	auto assign_options(v4_dhcp_options const& from, std::span<const std::uint8_t> which) -> v4_dhcp_packet&;
	auto assign_options(v4_dhcp_options const& from, std::initializer_list<const std::uint8_t> which) -> v4_dhcp_packet&;
	
	auto requested_parameters() const ->std::span<const std::uint8_t>;
	auto message_type(std::uint8_t msg_type) -> v4_dhcp_packet&;	
	auto message_type() const -> std::optional<std::uint8_t>;
	auto is_message_type(std::uint8_t msg_type) const -> bool;

	auto opcode() const->std::uint8_t;
	auto hardware_type() const->std::uint8_t;
	auto hardware_address_length() const->std::uint8_t;
	auto hardware_address() const->std::span<const std::uint8_t>;
	auto number_of_hops() const->std::uint8_t;
	auto client_address() const->std::uint32_t;
	auto your_address() const->std::uint32_t;
	auto server_address() const->std::uint32_t;
	auto gateway_address() const->std::uint32_t;
	auto transaction_id() const->std::uint32_t;
	auto seconds_elapsed() const->std::uint16_t;
	auto flags() const->std::uint16_t;
	auto server_host_name() const->std::string_view;
	auto boot_file_name() const->std::string_view;

	auto opcode(std::uint8_t value)->v4_dhcp_packet&;
	auto hardware_type(std::uint8_t value)->v4_dhcp_packet&;
	auto hardware_address(std::span<const std::uint8_t> value)->v4_dhcp_packet&;
	auto client_address(std::uint32_t value)->v4_dhcp_packet&;
	auto your_address(std::uint32_t value)->v4_dhcp_packet&;
	auto server_address(std::uint32_t value)->v4_dhcp_packet&;
	auto gateway_address(std::uint32_t value)->v4_dhcp_packet&;
	auto transaction_id(std::uint32_t value)->v4_dhcp_packet&;
	auto seconds_elapsed(std::uint16_t value)->v4_dhcp_packet&;
	auto flags(std::uint16_t value)->v4_dhcp_packet&;
	auto number_of_hops(std::uint8_t hops)->v4_dhcp_packet&;
	auto server_host_name(std::string_view value)->v4_dhcp_packet&;
	auto boot_file_name(std::string_view value)->v4_dhcp_packet&;
	
	auto serdes_size_hint() const->std::size_t;

	friend inline auto operator<<(std::ostream& oss, v4_dhcp_packet const& pkt) -> std::ostream&
	{
		return pkt.pretty_print(oss);
	}

protected:

	// Primary
	std::uint8_t		m_opcode { DHCP_OPCODE_RESPONSE };
	std::uint8_t		m_hardware_type { DHCP_HARDWARE_TYPE_ETHERNET };
	std::uint8_t		m_hardware_address_length { sizeof(mac_address_type) };
	std::uint8_t		m_number_of_hops { 0 };
	std::uint32_t		m_transaction_id { 0 };
	std::uint16_t		m_seconds_elapsed { 0 };
	std::uint16_t		m_flags { DHCP_FLAGS_BROADCAST };
	std::uint32_t		m_client_ip_address_v4  { 0 };
	std::uint32_t		m_your_ip_address_v4 { 0 };
	std::uint32_t		m_server_ip_address_v4 { 0 };
	std::uint32_t		m_gateway_ip_address_v4 { 0 };
	std::uint8_t		m_client_hardware_address [16] { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
	char						m_server_host_name [64] { 0 };
	char						m_boot_file_name [128] { 0 };
	v4_dhcp_options m_options;
};


