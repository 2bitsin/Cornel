#pragma once

#include <stop_token>
#include <thread>
#include <functional>
#include <filesystem>

#include <common/v4_address.hpp>
#include <common/config_ini.hpp>

#include  "tftp_packet.hpp"


struct tftp_server_v4;

struct tftp_session_v4
{
	using notify_func_type = std::function<void(tftp_session_v4 const*)>;

	template <typename P, typename T>
	tftp_session_v4(P& parent, v4_address source, T const& request)
	:	m_address		{ parent.address().port(0) },
		m_base_dir	{ parent.base_dir() },
		m_thread		{ [&parent, source, request, this] (auto st) { io_thread(parent, source, request, st); } }
	{}

	bool is_done() const;
	
	void io_thread(tftp_server_v4& parent, v4_address source, tftp_packet::type_rrq request, std::stop_token st);
	void io_thread(tftp_server_v4& parent, v4_address source, tftp_packet::type_wrq request, std::stop_token st);

	
private:
	std::atomic<bool> m_done{ false };
	v4_address m_address;
	std::filesystem::path m_base_dir;
	std::jthread m_thread;
};