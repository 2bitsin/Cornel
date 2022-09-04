#pragma once
#include <iostream>
#include <string>

static inline auto make_printable_string(auto&& value) -> std::string
{
	std::string result;
	for (auto chr : value)
	{
		if (chr >= ' ' && chr < 0x7f) 
		{
			result += chr;
			continue;
		}

		switch (chr)
		{
		case '\n': result += "\\n";
		case '\r': result += "\\r";
		case '\b': result += "\\b";
		case '\t': result += "\\t";
		case '\n': result += "\\n";

		}
	}
}

#define expect_eq(x, y) \
	return !([&] { \
		if (x != y) { \
			std::cerr << "Expected " #x " == \"" << (y) << "\", instead got : \"" << (x) << "\"\n"; \
			return false; \
		} \
		return true; \
	})()
	