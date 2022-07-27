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
static inline constexpr const char TARGET_PORT_A[] = R"(COM1)";
static inline constexpr const char TARGET_PORT_B[] = R"(COM2)";

void test_thread_runner(std::stop_token stt)
{
	using namespace std::string_literals;

	auto server = packet_io_connect::serial(TARGET_PORT_B, {
		.baud_rate = 115200,
		.data_bits = 8,
		.stop_bits = 1,
		.parity = packet_io_connect::parity_none
		});

	while (!stt.stop_requested())
	{
		try
		{
			auto packet = server->recv(3000);
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
		catch (std::exception const& ex)
		{
			std::cerr << ("ERROR: "s + ex.what() + "\n"s);
		}
	}
}

int main(int argc, char** argv)
{
	using namespace std::string_literals;
	using namespace std::string_view_literals;
	using namespace std::chrono_literals;
	try
	{
		std::stop_token stt;
		std::jthread test_thread{ test_thread_runner };
		std::this_thread::sleep_for(100ms);

		auto client = packet_io_connect::serial(TARGET_PORT_A, {
			.baud_rate = 115200,
			.data_bits = 8,
			.stop_bits = 1,
			.parity = packet_io_connect::parity_none
		});


		for (;;)
		{
			try
			{
				packet_buffer<byte> packet(128);
				packet.fill(0);
				packet.copy_from("Hello World !\n"sv);
				client->send(packet);
				
				auto echoed_packet = client->recv(3000);
				std::cout.write((char*)echoed_packet.data(), echoed_packet.size());
				std::this_thread::sleep_for(1s);
			}
			catch (generic_error const& ex)
			{
				std::cerr << ("ERROR: "s + ex.what() + "\n"s);
			}
			catch (std::exception const& ex)
			{
				std::cerr << ("ERROR: "s + ex.what() + "\n"s);
			}
		}

		return 0;
	}
	catch (generic_error const& ex)
	{
		std::cerr << ("ERROR: "s + ex.what() + "\n"s);
	}
	catch (std::exception const& ex)
	{
		std::cerr << ("ERROR: "s + ex.what() + "\n"s);
	}
}