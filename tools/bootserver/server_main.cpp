#include <filesystem>
#include <iostream>
#include <sstream>
#include <vector>
#include <stop_token>

#include "error.hpp"
#include "common/cxhash.hpp"
#include "common/trim.hpp"
#include "common/case.hpp"
#include "command_line.hpp"


int main(int argc, char** argv)
{
	for(auto&& arg : std::vector<std::string_view>{ argv, argv+argc })
	{
		std::cout << arg << '\n';
		
	}
	return 0;
}