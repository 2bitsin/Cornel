#pragma once

#include <cstdint>
#include <cstddef>
#include <span>
#include <string_view>

#include <vfsio/vfsio.hpp>
#include <vfsio/detail/archive.hpp>

namespace vfsio
{
  struct archive_view
	{
		using header_type = detail::archive_entry_header;
		using type_id_enum = detail::archive_entry_type_enum;

		static inline auto constexpr file = type_id_enum::file;
		static inline auto constexpr directory = type_id_enum::directory;		

		archive_view(std::span<std::byte const> bytes_v);
		
		auto find (error& error_v, std::string_view path_v) const -> std::tuple<std::span<std::byte const>, type_id_enum>;
		static auto find(error& error_v, std::string_view path_v, std::span<std::byte const> bytes_v) -> std::tuple<std::span<std::byte const>, type_id_enum>;

		auto find (error& error_v, type_id_enum type_v, std::string_view path_v) const -> std::span<std::byte const>;
		static auto find(error& error_v, type_id_enum type_v, std::string_view path_v, std::span<std::byte const> bytes_v) -> std::span<std::byte const>;
		
	protected:
		static auto header_from_bytes(std::span<std::byte const> bytes_v) -> header_type const*;
		static inline auto align(auto value_v) noexcept { 
			return (value_v + 0xfu) & ~(decltype(value_v))0xfu; 
		}
		

	private:
		std::span<std::byte const> m_bytes;	
	};

}