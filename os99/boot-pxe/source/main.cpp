#include <stdio.h>

#include <string>
#include <iostream>

#include "core/types.h"
#include "core/error.h"
#include "core/sysinfo.h"
#include "pxe/pxe.h"
#include "x86/debug.h"
#include "x86/flatmem.h"

static char const CORE_IMAGE_FILE_NAME[] = "os99core.img";

memmap_item far* locate_extended_memory(uint32_t size, system_info far* v_info)
{  
  memmap_item far* v_curr = NULL;
  uint16_t v_item_count = v_info->memory_map.size;
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

error_type download_image(char const far* file_name, system_info far* v_info) {

  PXE_status v_status = success;
  memmap_item far* v_extmem = NULL;    
  uint32_t v_file_size32 = 0;
  uint32_t v_down_size32 = 0;   
  v_status = PXE_query_size(CORE_IMAGE_FILE_NAME, &v_file_size32);
  if (v_status != success || v_file_size32 < 1) {
    return make_error(CORE_IMAGE_BAD_OR_NOT_FOUND);
  }
  v_extmem = locate_extended_memory(v_file_size32, v_info);
  if (!v_extmem) {
    return make_error(NOT_ENOUGH_EXTENDED_MEMORY);
  }
  printf("Downloading %s (%lu bytes)...", CORE_IMAGE_FILE_NAME, v_file_size32);  
  fflush(stdout);
  v_status = PXE_download(CORE_IMAGE_FILE_NAME, 
    v_extmem->base, v_extmem->size, &v_down_size32);
  if (v_status != success) {
    return make_error(PXE_DOWNLOAD_FAILED, v_status);
  }
  puts("Done.");
  v_info->image_addr = v_extmem->base;
  v_info->image_size = v_down_size32;  
  return NO_ERROR_SUCCESS;
}

int main(int, char**) 
{ 
  puts("BOOT-PXE v0.1 (BUILD: " __TIME__ " " __DATE__ ")");
  uint16_t v_status = 0;    
  v_status = PXE_init();
  if (v_status != success)
    halt_with_error(make_error(
      PXE_FAILED_TO_INITIALIZE, v_status));
  v_status = SYSINFO_acquire();
  system_info far* v_info = SYSINFO_info();
  if (v_status != NO_ERROR_SUCCESS)
    halt_with_error(v_status);  
  v_status = download_image(CORE_IMAGE_FILE_NAME, v_info);
  if (v_status != NO_ERROR_SUCCESS)
    halt_with_error(v_status);  
  return 0;
}


