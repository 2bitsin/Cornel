#include <vfsio/peloader.hpp>
#include <vfsio/pe32defs.hpp>
#include <utils/utils.hpp>

using vfsio::peloader;



auto peloader::load(error& error_v, std::span<std::byte const> raw_bytes_v) 
	-> std::tuple<void*, entrypoint_t*>
{
	using namespace pe32;
	using namespace utils;

#pragma pack(push, 1)
	struct image_headers
	{
		PE_dos_mz_header		dos_mz_header ;
		PE_coff_standard		standard_header ;
		PE_coff_optional_32 optional_header ;
		PE_data_directory		directory [IMAGE_NUMBEROF_DIRECTORY_ENTRIES];
	};	
#pragma pack(pop)
	
	error_v = error::none;
	static constexpr const std::tuple<void*, entrypoint_t*> null_v;

	auto const headers_v = std::make_unique<image_headers>();
	
	if (!partial_copy_from_bytes(headers_v->dos_mz_header, raw_bytes_v)) {
		error_v = error::invalid_or_corrupted;
		return null_v;
	}
	
	if (!partial_copy_from_bytes(headers_v->standard_header, raw_bytes_v, headers_v->dos_mz_header.e_lfanew)) {
		error_v = error::invalid_or_corrupted;
		return null_v;
	}

	if (IMAGE_FILE_MACHINE_I386 != headers_v->standard_header.machine) {
		error_v = error::unsupported_machine_type;
		return null_v;
	}
	
	if (sizeof(PE_coff_optional_32) > headers_v->standard_header.size_of_optional_header) {
		error_v = error::invalid_or_corrupted;
		return null_v;
	}

	auto const optional_header_offset_v = headers_v->dos_mz_header.e_lfanew + sizeof(PE_coff_standard);
	
	if (!partial_copy_from_bytes(headers_v->optional_header, raw_bytes_v, optional_header_offset_v)) {
		error_v = error::invalid_or_corrupted;
		return null_v;
	}

	if (IMAGE_NT_OPTIONAL_HDR32_MAGIC != headers_v->optional_header.magic) {
		error_v = error::invalid_or_corrupted;
		return null_v;
	}
	
	auto const data_directory_size_v = headers_v->standard_header.size_of_optional_header - sizeof(PE_coff_optional_32);	
	
	if (headers_v->optional_header.number_of_rva_and_sizes > IMAGE_NUMBEROF_DIRECTORY_ENTRIES) {
		error_v = error::invalid_or_corrupted;
		return null_v;
	}
	
	if (headers_v->optional_header.number_of_rva_and_sizes * sizeof(PE_data_directory) > data_directory_size_v) {
		error_v = error::invalid_or_corrupted;
		return null_v;
	}

	auto data_directory_offset_v = optional_header_offset_v + sizeof(PE_coff_optional_32);
	for (auto i = 0u; i < headers_v->optional_header.number_of_rva_and_sizes; ++i) {
		if (!partial_copy_from_bytes(headers_v->directory[i], raw_bytes_v, data_directory_offset_v)) {
			error_v = error::invalid_or_corrupted;
			return null_v;
		}
		data_directory_offset_v += sizeof(PE_data_directory);
	}

	auto const sections_s = from_bytes<PE_section_header>(raw_bytes_v, data_directory_offset_v, 
		headers_v->standard_header.number_of_sections * sizeof(PE_section_header));
	
	if (sections_s.empty()) {
		error_v = error::invalid_or_corrupted;
		return null_v;
	}	

	
	
	error_v = error::none;
	return null_v;
}