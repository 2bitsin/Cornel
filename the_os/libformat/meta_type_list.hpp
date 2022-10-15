#pragma once

#include <utility>
#include <cstdint>
#include <cstddef>

namespace meta
{
	// --------------------------
	// Type list
	// --------------------------
	
	template <typename... Item> 
	struct type_list 
	{
		static inline constexpr size_t size () { return sizeof...(Item); }
	};
	
	template <typename Item0, typename List>
	struct list_prepend_impl;
	
	template <typename Item0, typename... ItemN>
	struct list_prepend_impl<Item0, type_list<ItemN...>>
	{
		using type = type_list<Item0, ItemN...>;
	};
	
	template <typename Item0, typename List>
	using list_prepend = typename list_prepend_impl<Item0, List>::type;	
}