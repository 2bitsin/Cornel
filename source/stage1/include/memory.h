#ifndef __MEMORY_H__
#define __MEMORY_H__

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
  unsigned __int64 base;
  unsigned __int64 size;
  unsigned long type;
  unsigned long reserved;
} MEM_entry_type;

typedef struct MEM_entry_list_t MEM_entry_list_type;

struct MEM_entry_list_t 
{
  const MEM_entry_type entry;
  const MEM_entry_list_type* next;
  const MEM_entry_list_type* prev;
};

#pragma pack(pop)

int MEM_initialize();

void MEM_copy(const void* src, void* dst, unsigned int size);
void MEM_fill(void* dst, unsigned int size, unsigned char value);
void MEM_swap(void* lhs, void* rhs, unsigned int size);
void MEM_zero(void* dst, unsigned int size);

#endif

