#include <type_traits>
#include <iostream>
#include <string_view>
#include <string>

#include <concepts>

#include "meta_string.hpp"
#include "meta_type_list.hpp"
#include "meta_value_list.hpp"
#include "format_string.hpp"
#include "format_base_convert.hpp"


int main(int, char**)
{
  using namespace std::string_view_literals;
  using namespace std::string_literals; 

  //const auto s = ::textio::fmt::format<"Hello {{o{{ {1:x} {0:x}">();
	//std::cout << s << std::endl;

	using namespace textio::fmt::detail;

	constexpr auto v_u = string_convert_base_v<unsigned, "1234">;
	std::cout << v_u << "\n";
	
	constexpr auto v_i = string_convert_base_v<int, "-1234">;
	std::cout << v_i << "\n";
	
	//constexpr auto zz = meta::value_list_from_array<meta::string_truncate_v<"-1234">>{};
	//constexpr auto ss = meta::value_list_pop_front<meta::value_list_from_array<meta::string_truncate_v<"-1234">>> {};

	//std::cout << typeid(zz).name() << "\n";
	//std::cout << typeid(ss).name() << "\n";
  return 0;
}