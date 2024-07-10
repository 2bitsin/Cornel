#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include "core/error.h"
#include "x86/bios.h"
#include "x86/asm.h"
#include "pxe/pxe.h"

int main(int, char**) 
{     
  uint16_t v_status = 0;    
  puts("BOOT-PXE v0.1 (BUILD: " __TIME__ " " __DATE__ ")");
  v_status = PXE_init();
  if (v_status != success) {
    halt_with_error(make_combined_error(
      PXE_FAILED_TO_INITIALIZE, v_status));
    return -1;
  }  
  PXE_print_info();
      
  uint32_t v_size = 0;
  uint32_t v_next = 0;  

  memmap_item v_entry;
  v_next = 0;
  
  puts("------");
  printf("%-16s | %-16s | %-8s | %-8s\n", "BASE", "SIZE", "TYPE", "NEXT");
  puts("-----------------+------------------+----------+----------");
next_entry:
  v_size = sizeof(memmap_item);
  v_status = BIOS_query_memmap(&v_entry, &v_size, &v_next);
  if (v_status != 0) {
    halt_with_error(make_combined_error(
      CANT_READ_MEMORY_MAP, v_status));
    return -1;
  }
  printf("%08lX%08lX | %08lX%08lX | %08lX | %08lX\n", 
    (uint32_t)(v_entry.base >> 32), 
    (uint32_t)(v_entry.base & 0xFFFFFFFFu),
    (uint32_t)(v_entry.size >> 32), 
    (uint32_t)(v_entry.size & 0xFFFFFFFFu),
    v_entry.type, v_next);
  if (v_next != 0) {
    goto next_entry;
  }

  return 0;
}


