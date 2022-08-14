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
#include <unordered_set>

#include "v4_tftp_packet.hpp"
#include "v4_tftp_session.hpp"

struct v4_tftp_server
{
protected:
	using path = std::filesystem::path;
	using packet_queue = concurrent_queue<std::tuple<v4_address, std::vector<std::byte>>>;
	using notify_queue = concurrent_queue<v4_tftp_session const *>;
	using session_list = std::unordered_map<v4_tftp_session const *, std::unique_ptr<v4_tftp_session>>;
public:

	v4_tftp_server();
	v4_tftp_server(config_ini const&);
 ~v4_tftp_server();

	void initialize(config_ini const&);
  void start();
	void cease();

	auto address() const noexcept -> v4_address const&;
	auto base_dir() const noexcept -> path const&;
	auto session_notify(v4_tftp_session const* who) -> v4_tftp_server&;
	
private:
	
	void thread_incoming(std::stop_token st);
	void thread_outgoing(std::stop_token st);	
	 
	v4_address		m_address;
	path					m_base_dir;
	socket_udp		m_sock;
	packet_queue	m_packets;
	
	notify_queue	m_notify_queue;
	session_list	m_session_list;

	std::jthread	m_thread_incoming;
	std::jthread	m_thread_outgoing;
};