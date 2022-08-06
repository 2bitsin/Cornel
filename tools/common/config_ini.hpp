#pragma once

#include <iostream>
#include <unordered_map>
#include <any>
#include <string>
#include <string_view>
#include <optional>

struct config_ini
{
	config_ini();
	config_ini(std::istream& iss);
	config_ini(std::istream&& iss);

	auto parse(std::istream& iss) -> config_ini&;
		
	auto sections() const -> std::vector<std::string>;
	auto keys(std::string_view = "") const -> std::vector<std::string>;	
	auto get(std::string_view key, std::string_view section = "") const -> std::optional<std::string>;	

protected:
	auto parse_line(std::string_view line_sv, std::string& section_s) -> config_ini&;
	
private:
	std::unordered_map<std::string, std::unordered_map<std::string, std::string>> m_data;
};