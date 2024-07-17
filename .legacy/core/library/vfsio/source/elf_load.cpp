#include <utils/utils.hpp>
#include <vfsio/detail/elf.hpp>
#include <vfsio/elf_load.hpp>

auto vfsio::elf::load(vfsio::error& error_v, memory::block_list& block_list_v, std::span<std::byte const> bytes_v) -> image
{
	using namespace utils;
	const auto ehdr_p = partial_clone_from_bytes<Elf32_Ehdr>(bytes_v);	  

	if (!compare_array_to(ehdr_p->e_ident, '\x7f', 'E', 'L', 'F', ELFCLASS32, ELFDATA2LSB, EV_CURRENT, ELFOSABI_NONE)			
		||is_none_of<ET_EXEC, ET_DYN>(ehdr_p->e_type) 
		||ehdr_p->e_machine != EM_386 
		||ehdr_p->e_version != EV_CURRENT
		||ehdr_p->e_entry == 0u
		||ehdr_p->e_phoff < sizeof(Elf32_Ehdr)
		) 
	{	
		error_v = vfsio::error::not_supported;
		return {};
	}

	const auto phdr_p = partial_clone_from_bytes<Elf32_Phdr>(bytes_v, ehdr_p->e_phoff);

	return {};
}