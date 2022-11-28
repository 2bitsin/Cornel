#include <vfsio/peloader.hpp>
#include <filesystem>

#include "helpers.hpp"

int main(int, char**)
{
	memory::block_list heap_v {};
	static constexpr const auto Q = 64u*1024u*1024u;
	heap_v.insert_range({ new std::byte[Q], Q });
	vfsio::peloader peloader_v (heap_v);

	auto bytes_v = load(R"(C:\Users\alex\Desktop\projects\leisure\Cornel\workspace\os99\os99core.exe)");
	vfsio::error error_v;
	auto [image_p, entry_p] = peloader_v.load(error_v, bytes_v) ;

	return 0;
}