#include <type_traits>
#include <iostream>
#include <string_view>
#include <string>
#include <tuple>
#include <functional>

#include "meta_string.hpp"
#include "meta_list.hpp"


template <meta::string String>
struct format_static_type 
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
struct format_insert_type 
{
  static inline constexpr auto value = String;
  
  template <typename Collect, typename... Args>
  static void print(Collect& collect, std::tuple<Args...> const& args)
  {
		collect.append("D[");
		collect.append(value.as_string_view());
		collect.append("]\n");		
  }
};

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
		{
			return meta::list_prepend<format_insert_type<prev_string>, decltype (format_static<next_string>())> {};
		}
		else 
		{
			return meta::type_list<format_insert_type<prev_string>> {};
		}
  }
  else
  {
    return meta::type_list<format_insert_type<String>> {};
  }
}

template <meta::string String>
constexpr auto format_static() 
{
  constexpr auto index = String.template find<'{'>();
	if constexpr (index == String.npos)
  {
    return meta::type_list<format_static_type<String>> { };
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
			return meta::list_prepend<format_static_type<prev_string>, next_type> {};
		}
		else 
		{		
			return meta::type_list<format_static_type<prev_string>> {};
		}
	}
}

template <meta::string String>
constexpr auto format_encode()
{
  return format_static<meta::string_truncate_v<String>>();
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

	//static constexpr auto s1 = meta::string_truncate_v<"Hello ">;
	//static constexpr auto s2 = meta::string_truncate_v<"World!">;
	//
	//static constexpr meta::string s3 { s1, s2 };
	//
	//std::cout << s3.as_string_view() << "\n";
	//std::cout << typeid(decltype(s3)).name() << "\n";
  return 0;
}