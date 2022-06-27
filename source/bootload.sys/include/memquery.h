#ifndef __MEMQUERY_H__
#define __MEMQUERY_H__

#pragma pack(push, 1)

#define E820_ENTRY_TYPE_AVAILABLE        1
#define E820_ENTRY_TYPE_RESERVED         2
#define E820_ENTRY_TYPE_ACPI_RECLAIMABLE 3
#define E820_ENTRY_TYPE_ACPI_NVS         4

typedef struct MQ_e820_entry_type_t
{
  unsigned __int64 base;
  unsigned __int64 size;
  unsigned long type;
  unsigned long reserved;
} MQ_e820_entry_type;

typedef struct MQ_e820_params_type_t
{
  unsigned long size;
  unsigned long next;
  MQ_e820_entry_type entry;
} MQ_e820_params_type;

#pragma pack(pop)

int MQ_e820(MQ_e820_params_type* params);

#endif

