#include <functional>
#include <sstream>
#include <format>
#include <chrono>

#include <common/socket_error.hpp>
#include <common/logger.hpp>

#include "v4_dhcp_consts.hpp"
#include "v4_dhcp_server.hpp"

v4_dhcp_server::v4_dhcp_server()
{}

v4_dhcp_server::v4_dhcp_server(config_ini const& cfg)
: v4_dhcp_server()
{
	initialize(cfg);
}

v4_dhcp_server::~v4_dhcp_server()
{
	cease();
}

void v4_dhcp_server::initialize(config_ini const& cfg)
{
	using namespace std::string_view_literals;
	m_bind_address = v4_address(cfg.value_or("v4_bind_address"sv, "0.0.0.0"sv),
		lexical_cast<uint16_t>(cfg.value_or("dhcp_listen_port"sv, "67"sv)));

	for (auto&& client_mac : cfg.sections())
	{
		initialize_client(m_clients[std::string(client_mac)], cfg, client_mac);
	}
}

void v4_dhcp_server::start()
{
	using namespace std::chrono;
	
	Glog.info("Starting DHCP server, listening on '{}' ... ", m_bind_address.to_string());
	
	m_socket = m_bind_address.make_udp();
	m_socket.option<so_broadcast>(so_true);
	m_socket.timeout_recv(100ms);
	m_socket.timeout_send(100ms);

	m_thread_incoming = std::jthread([this](auto&& t){ thread_incoming (t); });	
	m_thread_outgoing = std::jthread([this](auto&& t){ thread_outgoing (t); });
}

void v4_dhcp_server::cease()
{
	Glog.info("Stopping DHCP server ... ");
	m_thread_incoming.request_stop();
	m_thread_outgoing.request_stop();
	if (m_thread_incoming.joinable())
		m_thread_incoming.join();
	if (m_thread_outgoing.joinable())
		m_thread_outgoing.join();
}

void v4_dhcp_server::thread_incoming(std::stop_token st)
{
	using namespace std::chrono_literals;
	Glog.info("* Receiver thread started.");
	while (!st.stop_requested())
	{
		try
		{
			auto [source, packet] = m_socket.recv(0);
			if (packet.size() < 1)
				continue;
			Glog.info("Received {} bytes from '{}'.", packet.size(), source.to_string());
			m_packets.push(std::tuple{ std::move(source), std::move(packet) });
		}
		catch (socket_error_timedout const& e)
		{ continue; }
		catch (stop_requested_error const& e)
		{ break; }
		catch (std::exception& ex)
		{
			Glog.error("{}", ex.what());
		}
	}
	Glog.info("* Receiver thread stopped.");
}

void v4_dhcp_server::thread_outgoing(std::stop_token st)
{
	Glog.info("* Responder thread started.");
	while (!st.stop_requested())
	{
		try
		{
			auto [source, packet_bits] = m_packets.pop(st);
			Glog.info("Responding to '{}'.", source.to_string());
			serdes<serdes_reader, network_byte_order> _serdes(std::span{ packet_bits });
			v4_dhcp_packet packet_s;
			_serdes(packet_s);
			
		#ifdef _DEBUG
			{
				std::ostringstream oss;
				oss << "\n\n" << packet_s;			
				Glog.info("{}", oss.str());
			}
		#endif

			if (packet_s.is_request())
				continue;
			
			if (packet_s.is_message_type(DHCP_MESSAGE_TYPE_DISCOVER))
			{
				const auto mac_address_v = mac_address_to_string (packet_s.hardware_address());
				const auto& offer_params_v = m_clients.at(mac_address_v);
				auto offer_packet = offer (packet_s, offer_params_v);
				// TODO : serialize packet
				continue;	
			}								
		}
		catch (socket_error_timedout const& e)
		{ continue; }
		catch (stop_requested_error const& e)
		{ break; }
		catch (std::exception const& ex)
		{
			Glog.error("{}", ex.what());
		}
	}
	Glog.info("* Responder thread stopped.");
}

void v4_dhcp_server::initialize_client(offer_params& client_v, config_ini const& cfg, std::string_view client_mac)
{
	using namespace std::string_view_literals;

	config_ini::section_type mac(client_mac);

	client_v.client_address = v4_parse_address(cfg.value_or(mac["v4_client_address"sv], "0.0.0.0"sv));
	client_v.server_address = v4_parse_address(cfg.value_or(mac["v4_server_address"sv], "0.0.0.0"sv));
	client_v.gateway_address = v4_parse_address(cfg.value_or(mac["v4_gateway_address"sv], "0.0.0.0"sv));
	client_v.server_host_name = cfg.value_or(mac["server_host_name"sv], ""sv);
	client_v.boot_file_name = cfg.value_or(mac["boot_file_name"sv], ""sv);

	client_v.dhcp_options.set(0x01u, v4_parse_address(cfg.value_or(mac["v4_subnet_mask"sv], "0.0.0.0"sv)));
	client_v.dhcp_options.set(0x03u, v4_parse_address(cfg.value_or(mac["v4_router_address"sv], v4_address_to_string(client_v.client_address))));
	client_v.dhcp_options.set(0x43u, std::span<char>(client_v.boot_file_name));
	client_v.dhcp_options.set(0x0Cu, std::span<char>(client_v.server_host_name));
}

auto v4_dhcp_server::offer(v4_dhcp_packet const& source_v, offer_params const& params_v) -> v4_dhcp_packet
{
	auto offered_packet_v = source_v;
	offered_packet_v.assign_options(params_v.dhcp_options, source_v.requested_parameters());
	offered_packet_v.message_type(DHCP_MESSAGE_TYPE_OFFER);
	offered_packet_v.server_address(params_v.server_address);
	offered_packet_v.client_address(params_v.client_address);
	offered_packet_v.your_address(params_v.client_address);
	offered_packet_v.gateway_address(params_v.gateway_address);
	offered_packet_v.boot_file_name(params_v.boot_file_name);
	offered_packet_v.server_host_name(params_v.server_host_name);		
	return offered_packet_v;
}

