#pragma  once

#include <string_view>
#include <stdexcept>

namespace textio::fmt::detail
{
	[[noreturn]] 
	static inline auto throw_conversion_error(std::string_view error) -> void
	{
		throw std::runtime_error(std::string(error));
	}
}