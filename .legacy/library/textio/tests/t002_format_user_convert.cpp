#include <iterator>
#include <textio/format.hpp>
#include "expect.hpp"

namespace textio::fmt
{
	struct vec3i
	{
		int x, y, z;
	
		inline auto format(auto&& vconv_r, auto&& options_v) const noexcept -> convert_error
		{
			return format_to<"vec3i({}, {}, {})">(vconv_r, x, y, z);
		}
	};
	
	struct box2u
	{
		unsigned x0,y0;
		unsigned x1,y1;
	};
	
	template <meta::string Options>
	struct user_convert<box2u, Options>
	{	
		template <typename T>
		static inline auto apply(T&& output_v, box2u const& value) -> convert_error
		{
			return format_to<"box2u[({0}, {1}), ({2}, {3})]">(std::forward<T>(output_v), value.x0, value.y0, value.x1, value.y1);
		}
	};	
}

int main(int,char** const) 
{
  using namespace std::string_literals;
  using namespace std::string_view_literals;
	using namespace textio::fmt;
	using namespace textio::fmt::literals;

	auto const expected = "vec3i(1, 2, 3)\nbox2u[(1, 2), (3, 4)]\n"s;
					
	const auto buffer = format_as<"{}\n{}\n", std::string>(vec3i{1,2,3}, box2u{1,2,3,4});
	
  expect_eq(buffer, expected);
}  

