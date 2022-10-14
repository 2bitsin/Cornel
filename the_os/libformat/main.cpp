#include <type_traits>
#include <iostream>
#include <string_view>
#include <string>
#include <tuple>
#include <functional>

#include "meta_string.hpp"
#include "meta_list.hpp"


template <meta::string String>
struct format_node_static 
{
  static inline constexpr auto value = String;
  
  template <typename Collect, typename... Args>
  static void print(Collect& collect, std::tuple<Args...> const& args)
  {
		collect.append("S[");
		collect.append(value.as_string_view());
		collect.append("]\n");		
  }
};

template <meta::string String>              
struct format_node_insert
{		
  static inline constexpr auto value = String;
  
  template <typename Collect, typename... Args>
  static void print(Collect& collect, std::tuple<Args...> const& args)
  {
		collect.append("I[");
		collect.append(value.as_string_view());
		collect.append("]\n");		
  }
};

template <typename Entity> struct format_is_static_impl: std::integral_constant<bool, false> {};
template <auto Value> struct format_is_static_impl<format_node_static<Value>>: std::integral_constant<bool, true> {};
template <typename Entity> concept format_is_static = format_is_static_impl<Entity>::value;

template <typename Node0>
constexpr auto format_optimize(meta::type_list<Node0> const&)
{
	return meta::type_list<Node0>{};
}

template <typename Node0, typename Node1, typename... NodeN>
requires (!format_is_static<Node0> || !format_is_static<Node1>)
constexpr auto format_optimize(meta::type_list<Node0, Node1, NodeN...> const&)
{
	return meta::list_prepend<Node0, decltype(format_optimize(meta::type_list<Node1, NodeN...>{}))>{};
}

template <typename Node0, typename Node1, typename... NodeN>
requires (format_is_static<Node0> && format_is_static<Node1>)
constexpr auto format_optimize(meta::type_list<Node0, Node1, NodeN...> const&)
{
	return format_optimize(meta::type_list<format_node_static<meta::string(Node0::value, Node1::value)>, NodeN...>{});		
}


template <meta::string String>
constexpr auto format_static();

template <meta::string String>
constexpr auto format_insert()
{
  constexpr auto index = String.template find<'}'>();
  if constexpr (index != String.npos)
	{
    constexpr auto prev_string = String.template substr<0, index>();
    constexpr auto next_string = String.template substr<index + 1u, String.size() - (index + 1u)>();
		
		if constexpr (!next_string.empty())
		{ return meta::list_prepend<format_node_insert<prev_string>, decltype (format_static<next_string>())> {}; }
		else 
		{ return meta::type_list<format_node_insert<prev_string>> {}; }
  }
  else
  { return meta::type_list<format_node_insert<String>> {}; }
}

template <meta::string String>
constexpr auto format_static() 
{
  constexpr auto index = String.template find<'{'>();
	if constexpr (index == String.npos)
  {
    return meta::type_list<format_node_static<String>> { };
  }   
	else 
	{
		constexpr auto esc = !(String[index + 1u] != '{');
		constexpr auto prev_string = String.template substr<0, index + esc>();
		constexpr auto next_string = String.template substr<index + 1u + esc, String.size() - (index + 1u + esc)>();
		if constexpr (!next_string.empty()) 
		{
			using next_type = std::conditional_t<!esc, 
				decltype (format_insert<next_string>()), 
				decltype (format_static<next_string>())>;
			return meta::list_prepend<format_node_static<prev_string>, next_type> {};
		}
		else 
		{		
			return meta::type_list<format_node_static<prev_string>> {};
		}
	}
}

template <meta::string String>
constexpr auto format_encode()
{
  return format_optimize(format_static<meta::string_truncate_v<String>>());
}

template <typename Collect, typename ArgsTuple, typename... NodeN>
auto format_impl(Collect& collect, meta::type_list<NodeN...>, ArgsTuple args)
{
  ((NodeN::print(collect, args)), ...);  
}

template <meta::string Format_string, typename... ArgN>
auto format(ArgN&&... args) -> std::string
{
  std::string collect;
  format_impl(collect, format_encode<Format_string>(), std::forward_as_tuple(std::forward<ArgN>(args)...));
  return collect;
}

int main(int, char**)
{
  using namespace std::string_view_literals;
  using namespace std::string_literals; 

  const auto s = format<"Hello {{o{{ {0:x}">();
	std::cout << s << std::endl;

	//using ni = format_node_insert<"Hello">;
	//using ns = format_node_static<"Hello">;
	//
	//static_assert(format_is_static<ni> == false);
	//static_assert(format_is_static<ns> == true);

	//static constexpr auto s1 = meta::string_truncate_v<"Hello ">;
	//static constexpr auto s2 = meta::string_truncate_v<"World!">;
	//
	//static constexpr meta::string s3 { s1, s2 };
	//
	//std::cout << s3.as_string_view() << "\n";
	//std::cout << typeid(decltype(s3)).name() << "\n";
  return 0;
}