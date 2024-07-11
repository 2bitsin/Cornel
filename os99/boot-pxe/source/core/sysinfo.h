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
    struct
    {
      uint8_t server_address[4];
      uint8_t gateway_address[4];
      uint8_t client_address[4];    
      char server_name[64];
      char boot_file[128];
      uint8_t client_mac[6];
    } dhcp_info;
    struct
    {
      uint16_t size;  
      memmap_item data[NUMBER_OF_MEMMAP_ITEMS];    
    } memory_map;
  };
#pragma pack(__pop)

error_type SYSINFO_acquire();
system_info far *SYSINFO_info();
