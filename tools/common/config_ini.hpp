#pragma once

#include <iostream>
#include <any>

struct config_ini
{
	config_ini();
	config_ini(std::istream& iss);

	auto parse(std::istream& iss) -> conf_ini&;
	
private:
	std::unordered_map<std::string, std::any> m_data;
};