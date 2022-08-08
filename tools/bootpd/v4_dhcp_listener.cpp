#include "v4_dhcp_listener.hpp"

#include <functional>
#include <format>
#include <chrono>

static std::string datetime_as_string(){
	std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::string s(30, '\0');
	std::strftime(&s[0], s.size(), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
	return s;
}

v4_dhcp_listener::v4_dhcp_listener()
{}

v4_dhcp_listener::~v4_dhcp_listener()
{
	cease();
}

void v4_dhcp_listener::initialize(config_ini const& cfg)
{	
	using namespace std::chrono;

	//m_client_address = v4_parse_address(cfg.get("v4_client_address").value_or("0.0.0.0"));
	//m_server_address = v4_parse_address(cfg.get("v4_server_address").value_or("0.0.0.0"));
	//m_gateway_address	= v4_parse_address(cfg.get("v4_gateway_address").value_or("0.0.0.0"));
	//m_boot_file_name = cfg.get("boot_file_name").value_or("");
	//m_server_host_name = cfg.get("server_host_name").value_or("");
	//
	//m_dhcp_options.set(0x01u, v4_parse_address(cfg.get("v4_subnet_mask").value_or("0.0.0.0")));
	//m_dhcp_options.set(0x03u, v4_parse_address(cfg.get("v4_router_address").value_or(v4_address_to_string(m_client_address))));
	//m_dhcp_options.set(0x43u, std::span<char>(m_boot_file_name));
	//m_dhcp_options.set(0x0Cu, std::span<char>(m_server_host_name));

	
	m_bind_address = v4_address(cfg.get("v4_bind_address").value_or("0.0.0.0"), 
		lexical_cast<uint16_t>(cfg.get("dhcp_listen_port").value_or("67")));
	m_socket = m_bind_address.make_udp();
	m_socket.option<so_broadcast>(so_true);
	m_socket.timeout_recv(100ms);
	m_socket.timeout_send(100ms);	
}

void v4_dhcp_listener::start()
{
	std::cout << std::format("{}: Starting DHCP server, listening on '{}' ... ", datetime_as_string(), m_bind_address.to_string());
	m_run_receiver = std::jthread(std::bind(&v4_dhcp_listener::run_receiver, this, std::placeholders::_1));
	m_run_responder = std::jthread(std::bind(&v4_dhcp_listener::run_responder, this, std::placeholders::_1));
	std::cout << "OK.\n";
}

void v4_dhcp_listener::cease()
{
	std::cout << std::format("{}: Stopping DHCP server ... ", datetime_as_string());
	m_run_receiver.request_stop();
	m_run_responder.request_stop();
	if (m_run_receiver.joinable())
		m_run_receiver.join();
	if (m_run_responder.joinable())
		m_run_responder.join();
	std::cout << "OK.\n";
}

void v4_dhcp_listener::run_receiver(std::stop_token st)
{
	using namespace std::chrono_literals;
	while (!st.stop_requested())
	{
		try
		{
			auto[source, packet] = m_socket.recv(0);
			if (packet.size() > 0) {
				std::cout << std::format("{}: Received {} bytes from '{}'.\n", datetime_as_string(), packet.size(), source.to_string());
				m_packets.push(std::tuple{ 
					std::move(source), 
					std::move(packet) 
				});
			}
		}
		catch (std::exception& ex)
		{
		//	std::cout << std::format("{}: Exception: {}\n", datetime_as_string(), ex.what());
		}
	}
}

void v4_dhcp_listener::run_responder(std::stop_token st)
{
	using namespace std::chrono_literals;
	while (!st.stop_requested())
	{
		auto[source, packet] = m_packets.pop();
		std::cout << std::format("{}: Responding to '{}'.\n", datetime_as_string(), source.to_string());
		//m_socket.send(source, );		
	}
}
