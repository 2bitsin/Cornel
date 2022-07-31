#include <iostream>
#include <string>
#include <vector>
#include <span>

#include "v4_address.hpp"

int main(int argc, char** argv)
{
	v4_address gogol ("google.com:8000");
	v4_address locol ("192.168.1.1:1234");

	std::printf("gogol => %s\n", gogol.to_string().c_str());
	std::printf("locol => %s\n", locol.to_string().c_str());
	return 0;
}

