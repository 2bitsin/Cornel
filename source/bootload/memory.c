#include "memory.h"
#include "print.h"
#include "error.h"

#pragma pack(push, 1)

typedef struct MEM_read_acpi_params_t
{
  unsigned long size;
  unsigned long next;
  MEM_entry_type entry;
} MEM_read_acpi_params_type;

#pragma pack(pop)

int MEM_read_acpi_entry(MEM_read_acpi_params_type* params);

unsigned int MEM_conv_memory_size ();

#pragma aux MEM_conv_memory_size = "int 0x12" value [ax];

void MEM_fill(void* dst, unsigned int size, unsigned char value)
{
  unsigned int i;
  for (i = 0; i < size; i++)
    ((unsigned char*)dst)[i] = value;
}

void MEM_copy(const void* src, void* dst, unsigned int size)
{
  const char* src_ = (const char*)src;
  char* dst_ = (char*)dst;

  if (src_ == dst_)
    return;

  if (src_ > dst_) {
    while (size--)
      *dst_++ = *src_++;
    return;
  }
  while (size--)
    *--dst_ = *--src_;
}

void MEM_swap(void* lhs, void* rhs, unsigned int size)
{
  char tmp;
  char* lhs_ = (char*)lhs;
  char* rhs_ = (char*)rhs;
  if (lhs_ == rhs_)
    return;
  if (lhs_ > rhs_) 
  {
    while (size--) 
    {
      tmp = *lhs_;
      *lhs_++ = *rhs_;
      *rhs_++ = tmp;
    }
    return;
  }
  while (size--) 
  {
    tmp = *--rhs_;
    *rhs_ = *--lhs_;
    *lhs_ = tmp;
  }  
}

void MEM_zero(void* dst, unsigned int size)
{
  MEM_fill(dst, size, 0);
}

static void MEM_print_acpi_entry(MEM_read_acpi_params_type* params)
{
  print_string("MEM: ");
  print_hex64(params->entry.base);
  print_string(" .. ");
  print_hex64(params->entry.base + params->entry.size - 1);
  print_string(" | ");
  print_hex64(params->entry.size);
  print_string(" | ");  
  switch(params->entry.type)
  {
  case MEM_ENTRY_TYPE_AVAILABLE: 
    print_string("Availble to OS"); 
    break;

  case MEM_ENTRY_TYPE_RESERVED: 
    print_string("Reserved"); 
    break;

  case MEM_ENTRY_TYPE_ACPI_RECLAIMABLE: 
    print_string("ACPI reclaimable memory"); 
    break; 

  case MEM_ENTRY_TYPE_ACPI_NVS: 
    print_string("ACPI NVS memory"); 
    break;

  default:
    print_string("Other (");    
    print_hex8(params->entry.type);
    print_char(')');
  }
  print_char('\n');
}

static int MEM_check_conventional_memory(unsigned long conv_memory_size)
{
#ifdef DEBUG
  print_string("MEM: ");
  print_dec16(conv_memory_size);
  print_string("KB of conventional memory found.\n\n");
#endif
  if (conv_memory_size < 576)
  {
    print_string("#0001 - 576KB of conventional memory is required.\n");  
    return MEM_ERROR_NOT_ENOUGH_CONVENTIONAL_MEMORY;
  }
  return 0;
}

int MEM_check_extended_memory(const unsigned __int64* extended_memory_size)
{
#ifdef DEBUG
  print_char('\n');
  print_string("MEM: Total ");
  print_hex64(*extended_memory_size);  
  print_string(" bytes of RAM\n");    
#endif
  if (*extended_memory_size < 0x2000000)
  {
    print_string("#0002 - 32 Megabytes of extended memory are required.\n");  
    return MEM_ERROR_NOT_ENOUGH_EXTENDED_MEMORY;
  }
  return 0;
}

void MEM_print_acpi_error(int error)
{
  print_string("#0003 - Can't query ACPI memory map.\n");
  print_string("#0003.1 - ");
  print_error_15h(error);  
  print_string("\n");
}

int MEM_populate_acpi_memory_map(unsigned __int64* out_memory_size)
{
  static MEM_read_acpi_params_type params;
  static unsigned __int64 total_memory_size;

  int error;

  total_memory_size = 0;
  params.next = 0;  
  do  
  {
    params.size = sizeof(params);
    error = MEM_read_acpi_entry(&params);
    if (error != 0) {
    #ifdef DEBUG
      MEM_print_acpi_error(error);      
    #endif
      return MEM_ERROR_ACPI_MEMORY_MAP_NOT_FOUND;
    }

    switch (params.entry.type)
    {
    case MEM_ENTRY_TYPE_AVAILABLE:
    case MEM_ENTRY_TYPE_ACPI_RECLAIMABLE:
      total_memory_size += params.entry.size;
      break;

    default:
      break;
    }
  #ifdef DEBUG
    MEM_print_acpi_entry(&params);
  #endif
  }
  while(params.next != 0);
  *out_memory_size = total_memory_size;
  return 0;
}

int MEM_initialize()
{
  static unsigned __int64 total_memory_size;
  unsigned int conv_memory_size;   
  int error;

  total_memory_size = 0;
  conv_memory_size = MEM_conv_memory_size();

  error = MEM_check_conventional_memory(conv_memory_size);
  if (error != MEM_ERROR_SUCCESS)
    return error;

  error = MEM_populate_acpi_memory_map(&total_memory_size);
  if (error != MEM_ERROR_SUCCESS)
    return error;    

  error = MEM_check_extended_memory(&total_memory_size);
  if (error != MEM_ERROR_SUCCESS)
    return error;    

  return 0;
}


