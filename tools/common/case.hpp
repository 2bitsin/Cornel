#pragma once

#include <algorithm>
#include <string>
#include <cctype>
#include <iterator>

template <typename What>
static inline constexpr auto lowercase(What&& what)
{
	std::basic_string value = std::forward<What>(what);
	std::transform(std::begin(value), std::end(value), std::begin(value), [](auto c) { return std::tolower(c); });
	return value;
}

template <typename What>
static inline constexpr auto uppercase(What&& what)
{
	std::basic_string value = std::forward<What>(what);
	std::transform(std::begin(value), std::end(value), std::begin(value),  [](auto c) { return std::toupper(c); });
	return value;
}


