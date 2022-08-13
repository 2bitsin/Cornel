#pragma once

#include <common/v4_address.hpp>
#include <common/socket_udp.hpp>
#include <common/config_ini.hpp>
#include <common/concurrent_queue.hpp>

#include <filesystem>
#include <vector>
#include <string>
#include <string_view>
#include <stop_token>
#include <tuple>

#include "v4_tftp_packet.hpp"

struct v4_tftp_server
{
	v4_tftp_server();
	v4_tftp_server(config_ini const&);
 ~v4_tftp_server();

	void initialize(config_ini const&);
  void start();
	void cease();
	
private:
	
	void thread_incoming(std::stop_token st);
	auto respond_with_error(v4_address const& to_whom, v4_tftp_packet::error_code_type errcode, std::string_view errstr) -> v4_tftp_server&;
	void thread_outgoing(std::stop_token st);

	using path = std::filesystem::path;
	using packet_queue = concurrent_queue<std::tuple<v4_address, std::vector<std::byte>>>;
	 
	packet_queue	m_packets;
	v4_address		m_address;
	socket_udp		m_sock;
	path					m_base_dir;

	std::jthread	m_thread_incoming;
	std::jthread	m_thread_outgoing;
};