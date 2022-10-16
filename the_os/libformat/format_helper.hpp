#pragma once

#include <type_traits>
#include <concepts>
#include "meta_string.hpp"
#include "meta_value_list.hpp"

namespace textio::fmt::detail
{

	
	template <auto... Haystack, typename Needle>
	requires (sizeof...(Haystack) > 1 && std::is_same_v<meta::value_list_front<meta::value_list<Haystack...>>, std::decay_t<Needle>>)
	constexpr auto is_any_of(Needle&& needle)
	{
		return ((needle == Haystack) || ...);
	}

	template <auto Haystack, typename Needle>
	requires requires () { { Haystack[0] } -> std::same_as<std::decay_t<Needle>>; }
	constexpr auto is_any_of(auto&& needle)
	{
		return ([]<std::size_t... Index>(auto&& needle, std::index_sequence<Index...> is) constexpr {
			return ((needle == Haystack[Index]) || ...);
		}) (std::forward<Needle>(needle), std::make_index_sequence<Haystack.size()>{});
	}
}