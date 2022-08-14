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

auto v4_tftp_server::address() const noexcept -> v4_address const&
{ return m_address; }

auto v4_tftp_server::base_dir() const noexcept -> path const&
{ return m_base_dir; }

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

auto v4_tftp_server::send_error(v4_address const& to_whom, v4_tftp_packet::error_code_type errcode)
	-> v4_tftp_server&
{
	using namespace std::string_view_literals;
	v4_tftp_packet packet;
	packet.set_error(errcode);
	m_sock.send(packet, to_whom, 0);	
	return *this;
}


auto v4_tftp_server::send_error(v4_address const& to_whom, v4_tftp_packet::error_code_type errcode, std::string_view errstr)
	-> v4_tftp_server&
{
	using namespace std::string_view_literals;
	v4_tftp_packet packet;
	packet.set_error(errcode, errstr);
	m_sock.send(packet, to_whom, 0);
	return *this;
}

auto v4_tftp_server::session_notify(v4_tftp_session const* who) -> v4_tftp_server&
{
	m_notify_queue.push(who);
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
			v4_tftp_packet packet_v (packet_bits);			
			Glog.info("From '{}' received : {} ", source.to_string(), packet_v.to_string());			

			packet_v.visit([this, source]<typename T>(T const& value)
			{
				if constexpr (std::is_same_v<T, v4_tftp_packet::packet_rrq_type>
					          ||std::is_same_v<T, v4_tftp_packet::packet_wrq_type>)
				{
					
					auto session_ptr = std::make_unique<v4_tftp_session>(*this, source, value);
					m_session_list.emplace(session_ptr.get(), std::move(session_ptr));
				}
				else if constexpr (std::is_same_v<T, std::monostate>)
					throw std::logic_error("Empty packet, packet parsing failed.");
				else {
					send_error(source, v4_tftp_packet::error_code_type::illegal_operation);
					Glog.info("Ignoring non-request packet from '{}'.", source.to_string());
				}
			});
			
			v4_tftp_session const* notify{ nullptr };
			while (m_notify_queue.try_pop(notify))
			{
				if (!(*notify).is_done())
					continue;				
				m_session_list.erase(notify);
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


