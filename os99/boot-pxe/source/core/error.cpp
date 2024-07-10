#include "core/error.h"
#include <stdio.h>
#include <stdlib.h>

char const far* error_code_to_string(error_code_type error_code, extended_error_code_type extended_error_code) 
{
#define X(y, x) case x: return #x
  switch (error_code) 
  {
  X(NO_ERROR_SUCCESS             , 0x00u);
  X(OUT_OF_MEMORY                , 0x01u);
  X(PXE_FAILED_TO_INITIALIZE     , 0x02u);
  X(CANT_READ_MEMORY_MAP         , 0x03u);
  X(ACQUIRE_BIOS_MEMMAP_FAILED   , 0x04u);
  X(ACQUIRE_BIOS_MEMMAP_OVERFLOW , 0x05u);
  default: return "UNDEFINED_ERROR_CODE";
  }
#undef X
}

combined_error_type make_combined_error (error_code_type main_code, extended_error_code_type ext_code) {
  return (combined_error_type)((((uint32_t)main_code)<<16u) + ext_code);
}

void halt_with_error(combined_error_type combined_error)
{
  error_code_type error_code = (error_code_type)(combined_error >> 16u);
  extended_error_code_type extended_error_code = (combined_error & 0xffffu);
  printf("BOOT FAILED : %04X-%04X %s\n", error_code, extended_error_code,
    error_code_to_string(error_code, extended_error_code));  
  exit(-1);
}