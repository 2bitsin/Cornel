#include <filesystem>

#include "helpers.hpp"

#include <vfsio/heapfile.hpp>
#include <vfsio/elf_load.hpp>
#include <memory/block_list.hpp>


int main(int, char**)
{
	memory::block_list heap_v {};
	static constexpr const auto Q = 64u*1024u*1024u;
	heap_v.insert_range({ new std::byte[Q], Q });
	
	auto bytes_v = load(R"(C:\Users\alex\Desktop\projects\leisure\Cornel\workspace\os99\os99core.elf)");
	vfsio::error error_v;
	vfsio::elf::load(error_v, heap_v, bytes_v);

	return 0;
}