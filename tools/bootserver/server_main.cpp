#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>
#include <span>
#include <thread>
#include <stop_token>

#include "packet_io.hpp"
#include "error.hpp"

static inline constexpr const char PIPE_NAME[] = R"(\\.\pipe\foobar)";

void test_thread_runner(std::stop_token stt)
{
	using namespace std::string_literals;	
	
	auto server = packet_io_connect::pipe(PIPE_NAME, packet_io_connect::as_server);
	
	while (!stt.stop_requested())
	{
		try
		{
			auto packet = server->recv();
			for (auto&& v : packet.bytes())
			{
				if (v != 0) v = std::toupper(v);
			}
			server->send(packet);
		}
		catch (generic_error const& ex)
		{
			std::cerr << ("ERROR: "s + ex.what() + "\n"s);
		}
	}
}

int main(int argc, char** argv)
try
{
	using namespace std::string_literals;
	using namespace std::string_view_literals;
	using namespace std::chrono_literals;

	std::stop_token stt;

	std::jthread test_thread{ test_thread_runner };
	
	std::this_thread::sleep_for(100ms);

	auto client = packet_io_connect::pipe(PIPE_NAME, packet_io_connect::as_client);


	for (;;)
	{
		try
		{
			packet_buffer<byte> packet(2048);			
			packet.fill(0);
			packet.copy_from("Hello World !\n"sv);			
			client->send(packet);							
			auto echoed_packet = client->recv();
			std::cout.write((char*)echoed_packet.data(), echoed_packet.size());
			std::this_thread::sleep_for(1s);
		}
		catch (generic_error const& ex)
		{
			std::cerr << ("ERROR: "s + ex.what() + "\n"s);
		}
	}
	
	return 0;
}
catch (generic_error const& ex)
{
	std::cerr << ex.what() << "\n";
}