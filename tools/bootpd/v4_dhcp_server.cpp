#include <functional>
#include <sstream>
#include <format>
#include <chrono>

#include <common/socket_error.hpp>
#include <common/logger.hpp>
#include <common/utility_case.hpp>

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
		initialize_client(m_clients[std::string(client_mac)], cfg, lowercase(std::string(client_mac)));
	}
}

void v4_dhcp_server::start()
{
	using namespace std::chrono;
	
	Glog.info("Starting DHCP server, listening on '{}' ... ", m_bind_address.to_string());
	
	m_socket = m_bind_address.make_udp();
	m_socket.option<so_broadcast>(so_true);
	m_socket.timeout(500ms);		
	m_thread_incoming = std::jthread([this](auto&& t){ thread_incoming (t); });	
	m_thread_outgoing = std::jthread([this](auto&& t){ thread_outgoing (t); });
	std::this_thread::sleep_for(10ms);
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
			v4_dhcp_packet packet_v(packet_bits);

			if (packet_v.opcode() != DHCP_OPCODE_REQUEST)
				continue;
			
			if (packet_v.is_message_type(DHCP_MESSAGE_TYPE_DISCOVER) 
			  ||packet_v.is_message_type(DHCP_MESSAGE_TYPE_REQUEST))
			{
				const auto mac_address_v = lowercase(mac_address_to_string (packet_v.hardware_address()));				
				const auto& offer_params_v = m_clients.at(mac_address_v);
				auto offer_packet_v = make_offer (packet_v, offer_params_v);
				
				if (packet_v.is_message_type(DHCP_MESSAGE_TYPE_DISCOVER)) {
					Glog.info("Responding to '{}' (transaction {:#08x}) DHCP.DISCOVER packet with DHCP.OFFER packet.", source.to_string(), packet_v.transaction_id());
					offer_packet_v.message_type(DHCP_MESSAGE_TYPE_OFFER);
				}
				else if (packet_v.is_message_type(DHCP_MESSAGE_TYPE_REQUEST)) {
					Glog.info("Responding to '{}' (transaction {:#08x}) DHCP.REQUEST packet with DHCP.ACK packet.", source.to_string(), packet_v.transaction_id());
					offer_packet_v.message_type(DHCP_MESSAGE_TYPE_ACK);
				}

				m_socket.send(offer_packet_v, v4_address::everyone().port(source.port()), 0u);
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

void v4_dhcp_server::initialize_client(offer_params& params_v, config_ini const& cfg, std::string_view client_mac)
{
	using namespace std::string_view_literals;

	config_ini::section_type mac(client_mac);

	params_v.client_address = v4_parse_address(cfg.value_or(mac["v4_client_address"sv], "0.0.0.0"sv));
	params_v.your_address = v4_parse_address(cfg.value_or(mac["v4_your_address"sv], "0.0.0.0"sv));
	params_v.server_address = v4_parse_address(cfg.value_or(mac["v4_server_address"sv], "0.0.0.0"sv));
	params_v.gateway_address = v4_parse_address(cfg.value_or(mac["v4_gateway_address"sv], "0.0.0.0"sv));
	params_v.server_host_name = cfg.value_or(mac["server_host_name"sv], ""sv);
	params_v.boot_file_name = cfg.value_or(mac["boot_file_name"sv], ""sv);

	params_v.dhcp_options.set(0x01u, v4_parse_address(cfg.value_or(mac["v4_subnet_mask"sv], "0.0.0.0"sv)));
	params_v.dhcp_options.set(0x03u, v4_parse_address(cfg.value_or(mac["v4_router_address"sv], v4_address_to_string(params_v.server_address))));
	params_v.dhcp_options.set(0x07u, v4_parse_address(cfg.value_or(mac["v4_log_server_address"sv], v4_address_to_string(params_v.server_address))));
	params_v.dhcp_options.set(0x0Cu, params_v.server_host_name);
	params_v.dhcp_options.set(0x0Fu, cfg.value_or(mac["domain_name"sv], "localhost"sv));
	
	params_v.dhcp_options.set(0x33u, cfg.value_or(mac["address_lease_time"sv], std::uint32_t(172800)));
	params_v.dhcp_options.set(0x36u, v4_parse_address(cfg.value_or(mac["v4_dhcp_server_address"sv], v4_address_to_string(params_v.server_address))));
	params_v.dhcp_options.set(0x3Au, cfg.value_or(mac["address_renewal_time"sv], std::uint32_t(86400)));
	params_v.dhcp_options.set(0x3Bu, cfg.value_or(mac["address_rebinding_time"sv], std::uint32_t(7200)));

	params_v.dhcp_options.set(0x43u, params_v.boot_file_name);	
}

auto v4_dhcp_server::make_offer(v4_dhcp_packet const& source_v, offer_params const& params_v) -> v4_dhcp_packet
{
	return (v4_dhcp_packet()
		.opcode(DHCP_OPCODE_RESPONSE)
		.hardware_type(DHCP_HARDWARE_TYPE_ETHERNET)
		.hardware_address(source_v.hardware_address())
		.number_of_hops(0)
		.flags(DHCP_FLAGS_BROADCAST)
		.seconds_elapsed(source_v.seconds_elapsed())
		.transaction_id(source_v.transaction_id())
		.client_address(params_v.client_address)
		.your_address(params_v.your_address)
		.server_address(params_v.server_address)
		.gateway_address(params_v.gateway_address)
		.boot_file_name(params_v.boot_file_name)
		.server_host_name(params_v.server_host_name)
		.assign_options(params_v.dhcp_options, source_v.requested_parameters())
		.assign_options(params_v.dhcp_options, { 54, 51, 58, 59, 7, 15 })
	);
}

