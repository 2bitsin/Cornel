#ifndef __MEMMAP_HPP__
#define __MEMMAP_HPP__

#include "types.hpp"

#pragma pack(push, 1)

struct e820_entry_type 
{
  uint64 base;
  uint64 size;
  uint32 type;
};

#pragma pack(pop)

void memory_load_map();

#endif