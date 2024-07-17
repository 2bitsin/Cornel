#pragma once

namespace meta
{	
	// --------------------------
	// Value list
	// --------------------------
	
	template <auto... Item> 
	struct value_list {};
	
	// -----------------------------
	// Value list from array
	// -----------------------------
	
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

	// -----------------------------------
	// Value list : algorithms : push_back
	// -----------------------------------

	template <typename ValueList, auto Item>
	struct value_list_push_back_impl;

	template <auto Item, auto... Value>
	struct value_list_push_back_impl<value_list<Value...>, Item>
	{
		using type = value_list<Value..., Item>;
	};

	template <typename ValueList, auto Item>
	using value_list_push_back = typename value_list_push_back_impl<ValueList, Item>::type;

	// --------------------------------
	// Value list : algorithms : front
	// --------------------------------

	template <typename ValueList>
	struct value_list_front_impl;

	template <auto Item0, auto... ItemN>
	struct value_list_front_impl<value_list<Item0, ItemN...>>
	{
		static inline constexpr auto value = Item0;
	};

	template <typename ValueList>
	static inline constexpr auto value_list_front = value_list_front_impl<ValueList>::value;

	// --------------------------------
	// Value list : algorithms : contains
	// --------------------------------

	template <typename ValueList, auto Item>
	struct value_list_contains_impl;
	
	template <auto Item, auto... Value>
	struct value_list_contains_impl<value_list<Value...>, Item>
	{
		static inline constexpr bool value = ((Item == Value) || ...);
	};

	template <typename ValueList, auto Item>
	static inline constexpr bool value_list_contains_v = value_list_contains_impl<ValueList, Item>::value;
	
	// -----------------------------------
	// Value list : algorithms : pop_front
	// -----------------------------------

	template <typename ValueList>
	struct value_list_pop_front_impl;

	template <auto Item0, auto... ItemN>
	struct value_list_pop_front_impl<value_list<Item0, ItemN...>>
	{
		using type = value_list<ItemN...>;
	};

	template <typename ValueList>
	using value_list_pop_front = typename value_list_pop_front_impl<ValueList>::type;
	

	// -----------------------------------
	// Value list : algorithms : transform
	// -----------------------------------

	template <typename List, template <auto> typename Map>
	struct value_list_transform_impl;
	
	template <auto... Item, template <auto> typename Map>
	struct value_list_transform_impl<value_list<Item...>, Map>
	{
		using type = value_list<Map<Item>::value...>;
	};

	template <typename List, template <auto> typename Map>
	using value_list_transform = typename value_list_transform_impl<List, Map>::type;
	

	// ---------------------------------
	// Value list : algorithms : reverse
	// ---------------------------------
	
	template <typename List>
	struct value_list_reverse_impl
	{
		using type = List;
	};
	
	template <auto Item0, auto... ItemN>
	struct value_list_reverse_impl<value_list<Item0, ItemN...>>
	{
		using type = value_list_push_back<typename value_list_reverse_impl<value_list<ItemN...>>::type, Item0>;
	};

	template <typename List>
	using value_list_reverse = typename value_list_reverse_impl<List>::type;
	
}
