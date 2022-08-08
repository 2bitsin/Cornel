#include <iostream>
#include <string>
#include <vector>
#include <span>
#include <fstream>
#include <filesystem>

#include "v4_dhcp_packet.hpp"
#include "v4_address.hpp"

#include <common/config_ini.hpp>
#include <common/lexical_cast.hpp>


int main(int argc, char** argv)
try
{
	std::filesystem::current_path(R"(C:\Users\alex\Desktop\projects\leisure\Cornel\tools\workspace)");
	config_ini _config(std::ifstream("config.ini"));

/*
	auto bind_to = v4_address("10.0.0.1", 67);	
	std::cout << "Starting server on " << bind_to.to_string() << " ...\n";
	auto dhcp_sock = bind_to.make_udp();	
	
	std::vector<std::byte> buffer_vec(8u*1024u, std::byte{});	
	std::span<std::byte> buffer_s = buffer_vec;	

	v4_address source;
	v4_dhcp_packet packet;
	dhcp_sock.option<so_broadcast>(so_true);
	while (dhcp_sock.recv(buffer_s, source, 0u))
	{
		try
		{
			std::cout << "\n **** \n\n";
			std::cout << "[[Received " << buffer_s.size() << " bytes from " << source.to_string() << "]]\n";

			serdes<serdes_reader, network_byte_order> _serdes(buffer_s);
			packet.serdes(_serdes);
			packet.pretty_print(std::cout);
			
			buffer_s = buffer_vec;
		}
		catch(std::exception const& ex)
		{ 
			std::cout << "I AM ERROR: " << ex.what() << "\n";
		}
	}
	*/
	return 0;
}
catch (std::exception const& ex)
{
	std::cerr << "Exception: " << ex.what() << std::endl;
	return 1;
}
catch (...)
{
	std::cerr << "Unknown exception" << std::endl;
	return 1;
}
