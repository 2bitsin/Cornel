#pragma once

#include <common/address_v4.hpp>
#include <common/socket_udp.hpp>
#include <common/config_ini.hpp>
#include <common/concurrent_queue.hpp>

#include <filesystem>
#include <vector>
#include <string>
#include <string_view>
#include <stop_token>
#include <tuple>
#include <unordered_set>

#include "tftp_packet.hpp"
#include "tftp_session_v4.hpp"

struct tftp_server_v4
{
protected:
	using path = std::filesystem::path;
	using packet_queue = concurrent_queue<std::tuple<address_v4, std::vector<std::byte>>>;
	using notify_queue = concurrent_queue<tftp_session_v4 const *>;
	using session_list = std::unordered_map<tftp_session_v4 const *, std::unique_ptr<tftp_session_v4>>;
public:

	tftp_server_v4();
	tftp_server_v4(config_ini const&);
 ~tftp_server_v4();

	void initialize(config_ini const&);
  void start();
	void cease();

	auto address() const noexcept -> address_v4 const&;
	auto base_dir() const noexcept -> path const&;
	auto session_notify(tftp_session_v4 const* who) -> tftp_server_v4&;
	
private:
	
	void thread_incoming(std::stop_token st);
	void thread_outgoing(std::stop_token st);	
	 
	address_v4		m_address;
	path					m_base_dir;
	socket_udp		m_sock;
	packet_queue	m_packets;
	
	notify_queue	m_notify_queue;
	session_list	m_session_list;

	std::jthread	m_thread_incoming;
	std::jthread	m_thread_outgoing;
};