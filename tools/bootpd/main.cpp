#include <iostream>
#include <string>
#include <vector>
#include <span>
#include <fstream>
#include <filesystem>

#include <common/address_v4.hpp>
#include <common/lexical_cast.hpp>
#include <common/control_c.hpp>
#include <common/config_ini.hpp>
#include <common/logger.hpp>
#include <common/arguments.hpp>

#include "dhcp_server_v4.hpp"
#include "tftp_server_v4.hpp"

int main(int argc, char** argv)
try
{
	using namespace std::string_literals;
	using namespace std::string_view_literals;
	{	
		
#ifndef NDEBUG
		std::filesystem::current_path(R"(C:\Users\alex\Desktop\projects\leisure\Cornel\workspace)");
		std::string config_path = "config.ini";		
#else
		arguments args(argc, argv);
		std::string config_path(args.value_or("-C"sv, "config.ini"sv));
#endif
		config_ini config_ini_v(std::ifstream{ config_path });
		
		dhcp_server_v4 dhcp_server_v (config_ini_v);		
		tftp_server_v4 tftp_server_v (config_ini_v);
		
		dhcp_server_v.start();
		tftp_server_v.start();
		
		while(!control_c::stop_requested())
		{
			using namespace std::chrono_literals;
			std::this_thread::sleep_for(1s);
		}
	}
	return 0;
}
catch (std::exception const& ex)
{
	Glog.fatal("{}", ex.what());
	return 1;
}
catch (...)
{
	Glog.fatal("Unknown unhandled exception");
	return 1;
}
