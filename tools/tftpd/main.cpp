#include <iostream>
#include <string>
#include <vector>
#include <span>

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
		std::cout << "Received " << buffer_s.size() << " bytes from " << source.to_string() << std::endl;
		buffer_s = buffer_vec;
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
