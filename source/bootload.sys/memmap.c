#include "memquery.h"
#include "print.h"
#include "error.h"

void print_e820_entry(MQ_e820_params_type* params)
{
  print_string("DEBUG: ");
  print_hex64(params->entry.base);
  print_string(" .. ");
  print_hex64(params->entry.base + params->entry.size - 1);
  print_string(" | ");
  print_hex64(params->entry.size);
  print_string(" | ");  
  switch(params->entry.type)
  {
  case E820_ENTRY_TYPE_AVAILABLE: 
    print_string("Availble to OS"); 
    break;

  case E820_ENTRY_TYPE_RESERVED: 
    print_string("Reserved"); 
    break;

  case E820_ENTRY_TYPE_ACPI_RECLAIMABLE: 
    print_string("ACPI reclaimable memory"); 
    break; 

  case E820_ENTRY_TYPE_ACPI_NVS: 
    print_string("ACPI NVS memory"); 
    break;

  default:
    print_string("Other (");    
    print_hex8(params->entry.type);
    print_char(')');
  }
  print_char('\n');
}

int memmap_e820()
{
  static unsigned __int64 total_size = 0;
  static MQ_e820_params_type params;
  
  int error;

  params.next = 0;
  do
  {
    params.size = sizeof(params);
    error = MQ_e820(&params);
    if (error != 0) {
    #ifdef DEBUG
      print_error_15h(error);
    #endif
      return error;
    }

    switch (params.entry.type)
    {
    case E820_ENTRY_TYPE_AVAILABLE:
    case E820_ENTRY_TYPE_ACPI_RECLAIMABLE:
      total_size += params.entry.size;
      break;

    default:
      break;
    }
  #ifdef DEBUG
    print_e820_entry(&params);
  #endif
  }
  while(params.next != 0);  
#ifdef DEBUG
  print_string("DEBUG: Total RAM: ");
  print_dec32(((unsigned long)total_size) / 1024);
  print_char(' ');
  print_string("KBytes");  
  print_char('\n');
#endif
  return 0;
}

