#include <stdexcept>
#include <string>
#include <string_view>
#include <mutex>
#include <system_error>
#include <cstdio>

#include "socket_api.hpp"
#include "byteorder.hpp"
#include "v4_address.hpp"

#define WIN32_LEAN_AND_MEAN
#define NOMINMAX

#include <ws2tcpip.h>
#include <WinSock2.h>
#include <Windows.h>

#pragma comment(lib, "ws2_32.lib")

//  TODO: WSAStartup

static void v4_initialize()
{
	static std::once_flag _;
	static WSADATA wsad;
	std::call_once(_, []() 
	{
		using namespace std::string_literals;
		RtlSecureZeroMemory(&wsad, sizeof(wsad));
		if (auto wsaerr = WSAStartup(MAKEWORD(2, 2), &wsad); wsaerr != 0)
			throw std::runtime_error("WSAStartup failed with code : "s + std::to_string(wsaerr));
		std::atexit([]() 
		{
			if (auto wsaerr = WSACleanup(); wsaerr != 0)
				std::fprintf(stderr, "WARNING! WSACleanup failed with code : %d!\n", wsaerr);
		});
	});
}

static auto v4_resolve(std::string_view target) -> std::uint32_t
{
	using namespace std::string_literals;
	std::string tmp{ target };

	v4_initialize();
	
	auto host_e = gethostbyname(tmp.c_str());
	if (!host_e || !host_e->h_addr_list[0])
		throw std::runtime_error(tmp + " cannot be resolved."s);
	if (host_e->h_addrtype != AF_INET)
		throw std::runtime_error(tmp + " cannot be resolved to a v4 address."s);
	return net_to_host(*(const uint32_t*)host_e->h_addr_list[0]);
}

void v4_init_sockaddr(sockaddr_in& target, std::size_t len, const struct v4_address& source)
{
	target.sin_family = AF_INET;
	target.sin_port = source.net_port();
	target.sin_addr.S_un.S_addr = source.net_addr();
}

void v4_init_sockaddr(sockaddr& target, std::size_t len, const struct v4_address& source)
{
	if(len < sizeof(sockaddr_in))
		throw std::logic_error("`target` is too small.");
	v4_init_sockaddr(*reinterpret_cast<sockaddr_in*>(&target), len, source);
}
auto v4_parse_address_and_port(std::string_view what) -> std::pair<uint32_t, uint16_t>
{	
	using namespace std::string_literals;
	uint32_t port{ 0 };
	in_addr address;
	std::string tmp;
		
	v4_initialize();
	
	if (auto it = what.find(':'); it != what.npos) {
		tmp = what.substr(it + 1);
		if (tmp.size() > 0)
		{
			std::size_t idx;
			port = std::stoul(tmp, &idx, 10);
			if (idx != tmp.size() || port > 65535)
				throw std::logic_error(tmp + " is not a valid port number.");
			port &= 0xffff;
		}
		what = what.substr(0, it);
	}		
	tmp = what;
	if (!inet_pton(AF_INET, tmp.c_str(), &address)) {
		return { v4_resolve(tmp), port };
	}
	return { net_to_host(address.S_un.S_addr), port };
}

auto v4_address_to_string(std::uint32_t address) -> std::string
{
	char buff [2048];
	in_addr addr_bits;	

	v4_initialize();
	
	std::memset(buff, 0, sizeof(buff));
	addr_bits.S_un.S_addr = host_to_net(address);
	if (!inet_ntop(AF_INET, &addr_bits, buff, sizeof(buff)))
		throw std::runtime_error("unable to convert address to string.");
	std::string tmp;
	tmp.assign(buff);
	return tmp;
}

auto v4_parse_address(std::string_view what) -> uint32_t
{
	auto[address, port] = v4_parse_address_and_port(what);
	return address;
}
