#pragma once

#include <thread>
#include <mutex>

#include <common/config_ini.hpp>
#include <common/lexical_cast.hpp>
#include <common/concurrent_queue.hpp>
#include <common/v4_address.hpp>
#include <common/socket_udp.hpp>

#include "v4_dhcp_options.hpp"
#include "v4_dhcp_packet.hpp"

struct v4_dhcp_server
{
	using packet_queue_type = concurrent_queue<std::tuple<v4_address, std::vector<std::byte>>>;
	
	v4_dhcp_server();
	v4_dhcp_server(config_ini const&);
 ~v4_dhcp_server();
	
	void initialize(config_ini const&);

	void start();
	void cease();
	

protected:
	struct offer_params
	{
		std::uint32_t			client_address;
		std::uint32_t			server_address;
		std::uint32_t			gateway_address;
		std::string				boot_file_name;
		std::string				server_host_name;	
		v4_dhcp_options		dhcp_options;			
	};	
	
	using client_map_type = std::unordered_map<std::string, offer_params>;

	void initialize_client(offer_params& client_v, config_ini const& cfg, std::string_view client_mac);

	auto make_offer(v4_dhcp_packet const& packet, offer_params const& client_v) -> v4_dhcp_packet;
	
private:
	void thread_incoming(std::stop_token st);
	void thread_outgoing(std::stop_token st);

	socket_udp					m_socket;	
	std::jthread				m_thread_incoming;
	std::jthread				m_thread_outgoing;		
	packet_queue_type		m_packets;
	v4_address					m_bind_address;
	client_map_type     m_clients;
};
