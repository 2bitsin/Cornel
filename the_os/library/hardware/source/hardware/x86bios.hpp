#pragma once

#include <cstddef>
#include <cstdint>
#include <system_error>

namespace x86arch
{
  #pragma pack(push, 1)
  struct alignas(16) bios_acpi_memory_map_entry_t
  {
    std::uint64_t base;
    std::uint64_t size;
    std::uint32_t type;
    std::uint32_t attr;
  };  
  #pragma pack(pop)

  auto bios_conventional_memory_size () -> std::uint32_t;
  auto bios_acpi_memory_map_read(bios_acpi_memory_map_entry_t& entry, std::uint32_t& o_length, std::uint32_t& o_offset, std::uint32_t i_offset = 0u) -> std::errc;
}