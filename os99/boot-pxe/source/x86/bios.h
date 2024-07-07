#pragma once

#include <stddef.h>
#include <stdint.h>

#pragma pack(__push, 1)
  struct memmap_entry
  {
    uint64_t base;
    uint64_t size;
    uint32_t type;
  };
#pragma pack(__pop)

uint16_t __watcall BIOS_query_memmap(memmap_entry far* v_buff, uint32_t *v_size, uint32_t *v_next);
