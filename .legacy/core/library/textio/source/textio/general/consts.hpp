#pragma once

#include <string_view>

namespace textio::consts
{
	using namespace std::string_view_literals;
	
	static inline const constexpr auto lf_c = '\n';
	static inline const constexpr auto cr_c = '\r';

	static inline const constexpr auto lf_s = "\n"sv;
	static inline const constexpr auto cr_s = "\r"sv;
	static inline const constexpr auto crlf_s = "\r\n"sv;
}
