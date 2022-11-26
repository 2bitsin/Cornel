#include <vfsio/peloader.hpp>
#include <vfsio/pe32defs.hpp>
#include <utils/utils.hpp>

using vfsio::peloader;

auto peloader::load(error& error_v, std::span<std::byte const> raw_bytes_v) 
	-> std::tuple<void*, entrypoint_t*>
{
	using namespace pe32;
	error_v = error::none;
	std::tuple<void*, entrypoint_t*> null_v;
	
	auto const msdosmz_header_p = utils::clone_from_bytes<PE_dos_mz_header>(raw_bytes_v);
	if (!msdosmz_header_p) {
		error_v = error::invalid_or_corrupted;
		return null_v;
	}

	auto const peimage_header_p = utils::clone_from_bytes<PE_coff_standard>(raw_bytes_v, msdosmz_header_p->e_lfanew);
	if (!peimage_header_p) {
		error_v = error::invalid_or_corrupted;
		return null_v;
	}

	if (IMAGE_FILE_MACHINE_I386 != peimage_header_p->machine) {
		error_v = error::unsupported_machine_type;
		return null_v;
	}

	if (peimage_header_p->size_of_optional_header < sizeof(PE_coff_optional_32)) {
		error_v = error::invalid_or_corrupted;
		return null_v;
	}

	auto const optional_header_p = utils::clone_from_bytes<PE_coff_optional_32>(raw_bytes_v, msdosmz_header_p->e_lfanew + sizeof(PE_coff_standard));

	
	
	error_v = error::none;
	return {nullptr, nullptr};
}