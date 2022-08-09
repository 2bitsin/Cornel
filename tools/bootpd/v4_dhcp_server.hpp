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

struct v4_dhcp_server
{
	using packet_queue_type = concurrent_queue<std::tuple<v4_address, std::vector<std::byte>>>;
	
	v4_dhcp_server();
	v4_dhcp_server(config_ini const&);
 ~v4_dhcp_server();
	
	void initialize(config_ini const&);

	void start();
	void cease();
	
private:
	void run_receiver(std::stop_token st);
	void run_responder(std::stop_token st);

protected:
	struct client
	{
		std::uint32_t			client_address;
		std::uint32_t			server_address;
		std::uint32_t			gateway_address;
		std::string				boot_file_name;
		std::string				server_host_name;	
		v4_dhcp_options		dhcp_options;			
	};	
	using client_map_type = std::unordered_map<std::string, client>;

	void initialize_client(client& client_v, config_ini const& cfg, std::string_view client_mac);

private:

	udp_socket					m_socket;	
	std::jthread				m_run_receiver;
	std::jthread				m_run_responder;		
	packet_queue_type		m_packets;
	v4_address					m_bind_address;
	client_map_type     m_clients;
};
