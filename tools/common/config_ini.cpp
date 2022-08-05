#include "config_ini.hpp"
#include "trim.hpp"
#include <string>
#include <string_view>

config_ini::config_ini(std::istream& iss)
{
	parse(iss);
}

config_ini::config_ini()
{}

config_ini::parse(std::istream& iss)
{
	std::string line_s;
	std::string_view line_sv;
	std::string_view::size_type pos;
	

	while (std::getline(iss, line_s))
	{
		line_sv = line_s;
		trim (line_s);		
		pos = line_sv.find('#');
		if (pos != line_sv.npos)
			line_sv.remove_suffix(pos);
		if (line_sv.empty())
			continue;
		
	}
}