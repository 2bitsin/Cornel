#pragma once


#include <common/config_ini.hpp>
#include <common/lexical_cast.hpp>
#include <common/concurrent_queue.hpp>

#include "v4_address.hpp"
#include "udp_socket.hpp"
#include "v4_dhcp_options.hpp"
#include "v4_dhcp_packet.hpp"

#include <thread>
#include <mutex>

struct v4_dhcp_listener
{
	using packet_queue_type = concurrent_queue<std::vector<std::byte>>;
	
	v4_dhcp_listener();
 ~v4_dhcp_listener();
	
	void initialize(config_ini const&);

	void start();
	void cease();
	
private:
	void run_receiver(std::stop_token st);
	void run_responder(std::stop_token st);
	
private:
	
	udp_socket				m_socket;	
	std::jthread			m_receiver_th;
	std::jthread			m_responder_th;		
	packet_queue_type m_packets;

	std::uint32_t			m_client_address;
	std::uint32_t			m_server_address;
	std::uint32_t			m_gateway_address;
	std::string				m_boot_file_name;
	std::string				m_server_host_name;	
	v4_dhcp_options		m_dhcp_options;	
};