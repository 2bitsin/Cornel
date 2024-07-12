#include <stdio.h>

#include <string>
#include <iostream>

#include "core/error.h"
#include "core/sysinfo.h"
#include "pxe/pxe.h"
#include "x86/debug.h"
#include "x86/flatmem.h"

memmap_item far* locate_extended_memory(uint32_t size)
{
  system_info far* v_info = SYSINFO_info();
  memmap_item far* v_curr = NULL;
  size_t v_item_count = v_info->memory_map.size;
  for (int i = 0; i < v_item_count; i+=1) {
    v_curr = &v_info->memory_map.data[i];
    if (v_curr->base < 0x100000 || 
        v_curr->type != 1 || 
        v_curr->size < size)
      continue;
    return v_curr;
  }
  return NULL;
}



int main(int, char**) 
{ 
  memmap_item far* v_extmem = NULL;    
  uint16_t v_status = 0;    
  uint32_t v_size32 = 0;
  puts("BOOT-PXE v0.1 (BUILD: " __TIME__ " " __DATE__ ")");
  v_status = PXE_init();
  if (v_status != success)
    halt_with_error(make_error(PXE_FAILED_TO_INITIALIZE, v_status));
  v_status = SYSINFO_acquire();
  if (v_status != NO_ERROR_SUCCESS)
    halt_with_error(v_status);
  v_status = PXE_query_size("os99core.img", &v_size32);
  if (v_status != success || v_size32 < 1) {
    halt_with_error(make_error(CORE_IMAGE_BAD_OR_NOT_FOUND));
  }
  printf("Downloading os99core.img (%lu bytes)...\n", v_size32);
  v_extmem = locate_extended_memory(v_size32);
  if (!v_extmem) {
    halt_with_error(make_error(NOT_ENOUGH_EXTENDED_MEMORY));
  }
  return 0;
}


