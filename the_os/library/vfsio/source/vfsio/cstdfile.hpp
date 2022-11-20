#pragma once

#include <cstdio>

#include <vfsio/vfsio.hpp>

namespace vfsio
{
	struct cstdfile final: 
		public vfsio::IFile
	{
		struct cstdfile_close { auto operator()(std::FILE* file_v) const { std::fclose(file_v); }};

		using cstdfile_ptr = std::unique_ptr<std::FILE, cstdfile_close>;

		cstdfile     (cstdfile_ptr file_v);
		cstdfile     (error& error_v,
									std::string_view path_v, 
						      std::string_view mode_v = "rb");
	 ~cstdfile     ();
	
		auto write   (error& error_v, std::span<std::byte const> buffer_v, std::uintmax_t offset_v) -> std::span<std::byte const> override;
		auto read    (error& error_v, std::span<std::byte> buffer_v, std::uintmax_t offset_v) -> std::span<std::byte> override;
		auto write   (error& error_v, std::span<std::byte const> buffer_v) -> std::span<std::byte const> override;
		auto read    (error& error_v, std::span<std::byte> buffer_v) -> std::span<std::byte> override;
				
		auto seek    (error& error_v, std::intmax_t offset_v, relative_to origin_v) -> std::uintmax_t override;
		auto tell    (error& error_v) const -> std::uintmax_t override;
		auto size    (error& error_v) const -> std::uintmax_t override;
		auto resize  (error& error_v, std::uintmax_t new_size_v) -> std::uintmax_t override;
		auto flush   (error& error_v) -> bool override;

	private:
		cstdfile_ptr m_file;
	};

}