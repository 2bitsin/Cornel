#pragma once

#include <cstddef>
#include <cstdint>
#include <system_error>

#include "x86bios_data_area.hpp"

namespace x86arch
{
  auto bios_conventional_memory_size () -> std::uint32_t;
  auto bios_acpi_memory_map_read (bios_acpi_memory_map_entry_t& entry, std::uint32_t& o_length, std::uint32_t& o_offset, std::uint32_t i_offset = 0u) -> std::errc;
  auto read_bios_data_area(std::uint8_t offset_i, void* value_o, std::size_t size_i) -> void;
}