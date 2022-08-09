#include <iostream>
#include <string>
#include <vector>
#include <span>
#include <fstream>
#include <filesystem>

#include <common/v4_address.hpp>
#include <common/lexical_cast.hpp>
#include <common/control_c.hpp>
#include <common/config_ini.hpp>
#include <common/logger.hpp>

#include "v4_dhcp_packet.hpp"
#include "v4_dhcp_server.hpp"


int main(int argc, char** argv)
try
{
	{
		std::filesystem::current_path(R"(C:\Users\alex\Desktop\projects\leisure\Cornel\tools\workspace)");
		config_ini config_ini_v(std::ifstream("config.ini"));		
		v4_dhcp_server dhcp_server_v (config_ini_v);		
		dhcp_server_v.start();			
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
