#include <utils/utils.hpp>
#include <vfsio/detail/elf.hpp>
#include <vfsio/elf_load.hpp>

auto vfsio::elf::load(vfsio::error& error_v, memory::block_list& block_list_v, std::span<std::byte const> bytes_v) -> image
{
	const auto ehdr_p = utils::partial_clone_from_bytes<vfsio::elf::Elf32_Ehdr>(bytes_v);	  
	
	

	return {};
}