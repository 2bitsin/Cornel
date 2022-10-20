#pragma once

#include <tuple>
#include <concepts>
#include <string>

#include "meta_type_list.hpp"
#include "meta_string.hpp"

#include "format_node_static.hpp"
#include "format_node_insert.hpp"
#include "format_optimize.hpp"
#include "format_parse.hpp"

namespace textio::fmt::detail
{
	template <meta::string String>
	inline constexpr auto format_encode()
	{
	  return format_optimize_empty(format_optimize_merge(format_static<meta::string_truncate_v<String>, 0u>()));
	}
	
	template <std::output_iterator<char> OIterator, typename ArgsTuple, typename... NodeN>
	inline auto format_to_impl(OIterator out_iterator, meta::type_list<NodeN...>, ArgsTuple const& args) -> OIterator
	{
		((out_iterator = NodeN::apply(out_iterator, args)), ...);
		return out_iterator;
	}
	
}

namespace textio::fmt
{
	template <meta::string Format_string, typename AsWhat = std::string, typename... ArgN>
	auto format_as(ArgN&&... args) -> AsWhat
	{
	  AsWhat collect;
	  detail::format_to_impl(std::back_inserter(collect), detail::format_encode<Format_string>(), std::forward_as_tuple(std::forward<ArgN>(args)...));
	  return collect;
	}

	template <meta::string Format_string, std::output_iterator<char> OIterator, typename... ArgN>
	auto format_to(OIterator o_iterator, ArgN&&... args) -> OIterator
	{
		return detail::format_to_impl(o_iterator, detail::format_encode<Format_string>(), std::forward_as_tuple(std::forward<ArgN>(args)...));
	}
}