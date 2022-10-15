#pragma once

#include <tuple>
#include <string>

#include "meta_type_list.hpp"
#include "meta_string.hpp"

#include "format_nodes.hpp"
#include "format_optimize.hpp"
#include "format_parse.hpp"

namespace textio::fmt::detail
{
	template <meta::string String>
	constexpr auto format_encode()
	{
	  return format_optimize_empty(format_optimize_merge(format_static<meta::string_truncate_v<String>>()));
	}
	
	template <typename Collect, typename ArgsTuple, typename... NodeN>
	auto format_impl(Collect& collect, meta::type_list<NodeN...>, ArgsTuple args)
	{
	  ((NodeN::print(collect, args)), ...);  
	}
}

namespace textio::fmt
{
	template <meta::string Format_string, typename... ArgN>
	auto format(ArgN&&... args) -> std::string
	{
	  std::string collect;
	  detail::format_impl(collect, detail::format_encode<Format_string>(), std::forward_as_tuple(std::forward<ArgN>(args)...));
	  return collect;
	}
}