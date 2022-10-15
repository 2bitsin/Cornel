#pragma once

namespace meta
{	
	// --------------------------
	// Value list
	// --------------------------
	
	template <auto... Item> 
	struct value_list {};
	
	template <auto Array, typename IndexSequece>
	struct value_list_from_array_impl;
	
	template <auto Array, std::size_t... Index>
	struct value_list_from_array_impl<Array, std::index_sequence<Index...>>
	{
		using type = value_list<Array[Index]...>;
		static inline constexpr size_t size () { return sizeof...(Index); }
	};

	template <auto Array>
	using value_list_from_array = typename value_list_from_array_impl<Array, std::make_index_sequence<std::size(Array)>>::type;
	

	// -----------------------------
	// Value list : algorithms : map
	// -----------------------------

	template <typename List, template <auto> typename Map>
	struct value_list_map_impl;
	
	template <auto... Item, template <auto> typename Map>
	struct value_list_map_impl<value_list<Item...>, Map>
	{
		using type = value_list<Map<Item>::value...>;
	};

	template <typename List, template <auto> typename Map>
	using value_list_map = typename value_list_map_impl<List, Map>::type;
	

	// ---------------------------------
	// Value list : algorithms : reverse
	// ---------------------------------
	
}