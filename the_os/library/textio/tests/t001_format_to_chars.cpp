#include <iterator>
#include <textio/format.hpp>
#include <textio/format/to_chars.hpp>
#include "expect.hpp"

#ifdef TESTING
#define main t000_format_to_chars_base_10
#endif

int main(int,char** const) 
{
  using namespace std::string_literals;
  using namespace std::string_view_literals;
	using namespace textio::fmt;
	using namespace textio::fmt::literals;

	auto const expected = ""s;
	char buff0 [128u];
	char buff1 [128u];

	constexpr auto Base = 2;

	std::string buffer0, buffer1;
	for(auto i = 0u; i <= 0xFFFFFFFFu; i += 367603u)
	{		
		
		auto [error0, end0] = detail::to_chars<true, Base>(std::begin(buff0), std::end(buff0), i);	
		buffer0.assign (std::string(std::begin(buff0), end0));
		
		auto [end1, error1] = std::to_chars(std::begin(buff1), std::end(buff1), i, Base);
		buffer1.assign (std::string(std::begin(buff1), end1));

		if (buffer0 != buffer1)
		{
			std::cerr << 
				"Expected " << buffer1 << "\n"
				" Instead " << buffer0 << "\n";
			__debugbreak();
			return 1;
		}
	}
	__debugbreak();
	return 0;
}  

