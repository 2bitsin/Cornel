#include <memory/allocator.hpp>
#include <memory>
#include "expect.hpp"

int main(int argc, char **grgv)
{
	auto buffer = std::make_unique<std::byte []> (1024u*1024u);

	allocator dut;

	dut.initialize({buffer.get(), 1024u * 1024u});

	std::visit ([]<typename T>(T const& value) 
	{
		if constexpr (std::is_same_v<T, void*>)
		{
			__debugbreak();
		}

		if constexpr (std::is_same_v<T, std::errc>)
		{
			__debugbreak();
		}
	}, dut.allocate(128u));

	
  return 0;
}