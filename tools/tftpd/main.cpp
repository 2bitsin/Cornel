#include <iostream>
#include <string>
#include <vector>
#include <span>

#include "v4_dhcp.hpp"
#include "v4_address.hpp"



int main(int argc, char** argv)
try
{
	auto dhcp_sock = v4_address::any(67).make_udp();	
	std::vector<std::byte> buffer_vec(8u*1024u, std::byte{});	
	std::span<std::byte> buffer_s = buffer_vec;	

	v4_address source;
	while (dhcp_sock.recv(buffer_s, source, 0u))
	{
		try
		{
			std::cout << "\n **** \n\n";
			std::cout << "[[Received " << buffer_s.size() << " bytes from " << source.to_string() << "]]\n";

			serdes<serdes_reader, network_byte_order> sdr(buffer_s);
			v4_dhcp packet{ sdr };

			packet.pretty_print(std::cout);
			
			buffer_s = buffer_vec;
		}
		catch(std::exception const& ex)
		{ }
	}

	
	
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
