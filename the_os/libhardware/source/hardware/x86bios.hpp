#pragma once

#include <cstddef>
#include <cstdint>

namespace x86arch
{
  #pragma pack(push, 1)
  struct bios_acpi_memory_map_entry_t
  {
    std::uint64_t base;
    std::uint64_t size;
    std::uint32_t type;
  };  
  #pragma pack(pop)

  auto bios_conventional_memory_size () -> std::uint32_t;
  auto bios_acpi_memory_map_read(bios_acpi_memory_map_entry_t& entry, std::uint32_t last = 0u) -> std::uint32_t;
}