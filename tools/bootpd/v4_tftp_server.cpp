#include <chrono>
#include <functional>
#include <filesystem>

#include <common/logger.hpp>
#include <common/v4_address.hpp>
#include <common/socket_udp.hpp>
#include <common/socket_error.hpp>

#include "v4_tftp_server.hpp"
#include "v4_tftp_packet.hpp"
#include "v4_tftp_consts.hpp"


v4_tftp_server::v4_tftp_server()
{
}

v4_tftp_server::v4_tftp_server(config_ini const& cfg)
{
	initialize(cfg);
}

v4_tftp_server::~v4_tftp_server()
{
	cease();
}

void v4_tftp_server::initialize(config_ini const& cfg)
{
	using namespace std::string_view_literals;
	m_address = cfg.value_or("v4_bind_address"sv, v4_address::any()).port(cfg.value_or("tftp_listen_port"sv, 69));
	m_base_dir = cfg.value_or("tftp_base_dir"sv, std::filesystem::path("./"));	
}

void v4_tftp_server::start()
{
	using namespace std::chrono_literals;
	Glog.info("Starting TFTP server on '{}' ... ", m_address.to_string());
	m_sock = m_address.make_udp();
	m_sock.timeout(500ms);	
	m_thread_incoming = std::jthread([this](auto&& st){ thread_incoming (st); });
	m_thread_outgoing = std::jthread([this](auto&& st){ thread_outgoing (st); });
	std::this_thread::sleep_for(10ms);
}

void v4_tftp_server::cease()
{
	Glog.info("Stopping TFTP server on '{}' ... ", m_address.to_string());
	if (m_thread_incoming.joinable()) {
		m_thread_incoming.request_stop();
		m_thread_incoming.join();
	}
	if (m_thread_outgoing.joinable()) {
		m_thread_outgoing.request_stop();
		m_thread_outgoing.join();
	}
}

void v4_tftp_server::thread_incoming(std::stop_token st)
{
	using namespace std::string_view_literals;
	Glog.info("* Receiver thread started.");
	while (!st.stop_requested()) 
	{
		try
		{
			auto[source, packet_bits] = m_sock.recv(0);
			if (packet_bits.empty()) 
				continue;
			Glog.info("Received {} byte TFTP packet from '{}' ... ", packet_bits.size(), source.to_string());
			m_packets.emplace(
				std::move(source), 
				std::move(packet_bits)
			);
		}
		catch (socket_error_timedout const&)
		{ continue; }
		catch (std::exception const& e)
		{ Glog.error("{}"sv, e.what()); }
	}
	Glog.info("* Receiver thread stopped.");
}

auto v4_tftp_server::respond_with_error(v4_address const& to_whom, v4_tftp_packet::error_code_type errcode, std::string_view errstr)
	-> v4_tftp_server&
{
	using namespace std::string_view_literals;
	v4_tftp_packet packet;
	packet.set_error(errcode, errstr);
	m_sock.send(packet, to_whom, 0);
	return *this;
}

void v4_tftp_server::thread_outgoing(std::stop_token st)
{
	Glog.info("* Responder thread started.");
	using namespace std::string_view_literals;

	while (!st.stop_requested()) 
	{
		try
		{							
			auto[source, packet_bits] = m_packets.pop(st);
			v4_tftp_packet tftp_packet_v;
			(serdes<serdes_reader>(std::span{ packet_bits }))(tftp_packet_v);
			Glog.info("From '{}' received : {} ", source.to_string(), tftp_packet_v.to_string());			

			if (tftp_packet_v.opcode() != TFTP_OPCODE_RRQ &&
				  tftp_packet_v.opcode() != TFTP_OPCODE_WRQ)
			{
				Glog.info("* Unexpected packet with opcode {} from '{}' ...", tftp_packet_v.opcode(), source.to_string());				
				continue;
			}
		}
		catch (socket_error_timedout const&)
		{ continue; }
		catch (stop_requested_error const&)
		{ break; }
		catch (std::exception const& e)
		{ Glog.error("{}"sv, e.what()); }		
	}
	Glog.info("* Responder thread stopped.");
}


