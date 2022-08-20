#include "types.h"
#include "flatreal.h"

#pragma pack(push, 1)

const uint64_t GDT_bits [] =
{
  0x0000000000000000,  // 0x0000 = null
  0x00cf9e000000ffff,  // 0x0008 = 32bit code 
  0x00cf92000000ffff,  // 0x0010 = 32bit data/stack
  0x00009e000000ffff,  // 0x0018 = 16bit code
  0x000092000000ffff   // 0x0020 = 16bit data/stack
};

const size_t GDT_size = sizeof(GDT_bits);

#pragma pack(pop)
