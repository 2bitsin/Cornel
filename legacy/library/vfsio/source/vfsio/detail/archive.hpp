#pragma once

#include <cstddef>
#include <cstdint>

namespace vfsio::detail
{

  enum archive_entry_type_enum: std::uint8_t
  {
		undefined = 0xffu,
    directory = 0x00u,
    file      = 0x01u
  };

#pragma pack(push, 1)
  struct archive_entry_header
  {
    std::uint32_t size;
    archive_entry_type_enum type_id;
    std::uint8_t name_len;
    char name[250];

		inline constexpr auto header_size() const noexcept { return offsetof(archive_entry_header, name) + name_len; }
  };
#pragma pack(pop) 

}