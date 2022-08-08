#include "v4_dhcp_listener.hpp"

v4_dhcp_listener::v4_dhcp_listener()
{}

void v4_dhcp_listener::initialize(config_ini const& cfg)
{	
	using namespace std::chrono;

	m_client_address		= v4_parse_address(cfg.get("v4_client_address").value_or("0.0.0.0"));
	m_server_address		= v4_parse_address(cfg.get("v4_server_address").value_or("0.0.0.0"));
	m_gateway_address		= v4_parse_address(cfg.get("v4_gateway_address").value_or("0.0.0.0"));
	m_boot_file_name		= cfg.get("boot_file_name").value_or("");
	m_server_host_name	= cfg.get("server_host_name").value_or("");
	
	m_dhcp_options.set(0x01u, v4_parse_address(cfg.get("v4_subnet_mask").value_or("0.0.0.0")));
	m_dhcp_options.set(0x03u, v4_parse_address(cfg.get("v4_router_address").value_or(v4_address_to_string(m_client_address))));
	m_dhcp_options.set(0x43u, std::span<char>(m_boot_file_name));
	m_dhcp_options.set(0x0Cu, std::span<char>(m_server_host_name));

	v4_address interface_address (cfg.get("v4_interface_address").value_or("0.0.0.0"),
		                            lexical_cast<uint16_t>(cfg.get("dhcp_listen_port").value_or("67")));
	m_socket = interface_address.make_udp();
	m_socket.option<so_broadcast>(so_true);
	m_socket.timeout_recv(100ms);
	m_socket.timeout_send(100ms);	
}
