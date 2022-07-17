#pragma once

#include <string_view>
#include <string>
#include <vector>
struct command_line
{
	static void parse(std::string_view line, std::vector<std::string>& parts);
};
