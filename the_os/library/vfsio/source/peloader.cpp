#include <vfsio/peloader.hpp>
#include <vfsio/pe32defs.hpp>
#include <utils/utils.hpp>

using vfsio::peloader;

#pragma pack(push, 1)
struct data_directory
{
	vfsio::pe32::PE_data_directory array [vfsio::pe32::IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
};
#pragma pack(pop)


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

	auto const pe_image_header_p = utils::clone_from_bytes<PE_coff_standard>(raw_bytes_v, msdosmz_header_p->e_lfanew);
	
	if (!pe_image_header_p) {
		error_v = error::invalid_or_corrupted;
		return null_v;
	}

	if (IMAGE_FILE_MACHINE_I386 != pe_image_header_p->machine) {
		error_v = error::unsupported_machine_type;
		return null_v;
	}

	if (pe_image_header_p->size_of_optional_header < sizeof(PE_coff_optional_32)) {
		error_v = error::invalid_or_corrupted;
		return null_v;
	}
	auto const optional_header_offset_v = msdosmz_header_p->e_lfanew + sizeof(PE_coff_standard);
	auto const optional_header_p = utils::clone_from_bytes<PE_coff_optional_32>(raw_bytes_v, optional_header_offset_v);

	if (IMAGE_NT_OPTIONAL_HDR32_MAGIC != optional_header_p->magic) {
		error_v = error::invalid_or_corrupted;
		return null_v;
	}
	
	auto const data_directory_offset_v = optional_header_offset_v + sizeof(PE_coff_optional_32);
	auto const data_directory_size_v = pe_image_header_p->size_of_optional_header - sizeof(PE_coff_optional_32);

	auto const data_directory_s = utils::clone_from_bytes<data_directory>(raw_bytes_v, 
		data_directory_offset_v, data_directory_size_v);


	
	
	error_v = error::none;
	return null_v;
}