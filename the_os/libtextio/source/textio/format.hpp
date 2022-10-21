#pragma once

#include <tuple>
#include <concepts>
#include <string>
#include <iterator>

#include "meta/type_list.hpp"
#include "meta/string.hpp"

#include "format/node_static.hpp"
#include "format/node_insert.hpp"
#include "format/optimize.hpp"
#include "format/parse.hpp"
#include "general/cstdio_iterator.hpp"

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
	template <meta::string Format_string = meta::string{"{}"}, typename AsWhat = std::string, typename... ArgN>
	auto format_as(ArgN&&... args) -> AsWhat
	{
	  AsWhat collect;
	  detail::format_to_impl(std::back_inserter(collect), detail::format_encode<Format_string>(), std::forward_as_tuple(std::forward<ArgN>(args)...));
	  return collect;
	}

	template <meta::string Format_string = meta::string{"{}"}, std::output_iterator<char> OIterator, typename... ArgN>
	auto format_to(OIterator o_iterator, ArgN&&... args) -> OIterator
	{
	
	return detail::format_to_impl(o_iterator, detail::format_encode<Format_string>(), std::forward_as_tuple(std::forward<ArgN>(args)...));
	}
	
	template <meta::string Format_string = meta::string{"{}"}, typename... ArgN>
	auto format_to(std::FILE* file, ArgN&&... args) -> int
	{
		using textio::detail::cstdio_iterator;
		auto cstdio_i = detail::format_to_impl(cstdio_iterator{ file }, detail::format_encode<Format_string>(), std::forward_as_tuple(std::forward<ArgN>(args)...));
		return cstdio_i.status();
	}
}

namespace textio::fmt
{

	template <meta::string Format_string>
	struct format_statement
	{
		constexpr format_statement() = default;
		using char_type = typename decltype(Format_string)::char_type;

		static inline constexpr auto format_string_encoded = detail::format_encode<Format_string>();

		template <std::output_iterator<char> OIterator, typename... ArgN>
		static inline auto to(OIterator o_iterator, ArgN&& ... args) -> OIterator
		{
			return detail::format_to_impl(o_iterator, format_string_encoded, std::forward_as_tuple(std::forward<ArgN>(args)...));
		}		

		template <typename... ArgN>
		static inline auto to(std::basic_string<char_type>& o_string, ArgN&& ... args) -> std::basic_string<char_type>
		{
			detail::format_to_impl(std::back_inserter(o_string), format_string_encoded, std::forward_as_tuple(std::forward<ArgN>(args)...));
			return o_string;
		}		

		template <typename... ArgN>
		static inline auto to(FILE* o_file, ArgN&& ... args) -> int
		{
			using ::textio::detail::cstdio_iterator;
			auto cstdio_i = detail::format_to_impl(cstdio_iterator{ o_file }, format_string_encoded, std::forward_as_tuple(std::forward<ArgN>(args)...));
			return cstdio_i.status();
		}		

		template <typename As_type, typename... ArgN>
		static inline auto as(ArgN&& ... args)
		{
			As_type collect;			
			return to(collect, std::forward<ArgN>(args)...);
		}		

		template <std::output_iterator<char> OIterator, typename... ArgN>
		inline auto operator () (std::output_iterator<char> auto o_iterator, ArgN&& ... args) const -> OIterator
		{
			return to(o_iterator, std::forward<ArgN>(args)...);
		}		

		template <typename... ArgN>
		inline auto operator () (std::basic_string<char_type>& o_string, ArgN&& ... args) const -> std::basic_string<char_type>
		{			
			return to(o_string, std::forward<ArgN>(args)...);
		}
		
		template <typename... ArgN>
		inline auto operator () (std::FILE* o_file, ArgN&& ... args) const -> int
		{			
			return to(o_file, std::forward<ArgN>(args)...);
		}
	};

	namespace literals
	{
		template<meta::string Format_string>
		constexpr auto operator "" _fmt()
		{
			return format_statement<Format_string>();
		}
	}
}