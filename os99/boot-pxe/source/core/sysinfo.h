#pragma once

#include <stddef.h>
#include <stdint.h>

#include "core/error.h"

#pragma pack(__push, 1)
  struct memmap_item
  {
    uint64_t base;
    uint64_t size;
    uint32_t type;
    uint32_t attr;
  };

#define NUMBER_OF_MEMMAP_ITEMS 32

  struct system_info
  {
    uint8_t _reseved0[7];
    uint8_t memory_map_size;
    memmap_item memory_map[NUMBER_OF_MEMMAP_ITEMS];    
  };
#pragma pack(__pop)

combined_error_type SYSINFO_acquire();
