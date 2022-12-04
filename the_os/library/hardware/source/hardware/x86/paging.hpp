#pragma once

#include <cstdint>
#include <cstddef>
#include <span>

struct Paging
{
  auto map (void* virtual_addr_v, std::uint32_t physical_addr_v, std::size_t size_v, std::uint32_t access_v) -> bool;

};