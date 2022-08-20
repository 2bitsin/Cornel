#ifndef __MEMORY_H__
#define __MEMORY_H__

#include "types.h"

#pragma pack(push, 1)

#define MEM_ENTRY_TYPE_AVAILABLE        1
#define MEM_ENTRY_TYPE_RESERVED         2
#define MEM_ENTRY_TYPE_ACPI_RECLAIMABLE 3
#define MEM_ENTRY_TYPE_ACPI_NVS         4

#define MEM_ERROR_SUCCESS                          0
#define MEM_ERROR_NOT_ENOUGH_CONVENTIONAL_MEMORY  -1
#define MEM_ERROR_NOT_ENOUGH_EXTENDED_MEMORY      -2
#define MEM_ERROR_ACPI_MEMORY_MAP_NOT_FOUND       -3

typedef struct MEM_entry_t
{
  uint64_t base;
  uint64_t size;
  uint32_t type;
  uint32_t reserved;
} MEM_entry_type;

typedef struct MEM_entry_list_t MEM_entry_list_type;

struct MEM_entry_list_t 
{
  const MEM_entry_type entry;
  const MEM_entry_list_type* next;
  const MEM_entry_list_type* prev;
};

#pragma pack(pop)

int MEM_init();

void MEM_copy(const void* src, void* dst, size_t size);
void MEM_fill(void* dst, size_t size, uint8_t value);
void MEM_swap(void* lhs, void* rhs, size_t size);
void MEM_zero(void* dst, size_t size);

#endif

