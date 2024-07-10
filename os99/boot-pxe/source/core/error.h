#pragma once

#include <stdint.h>
#include <stddef.h>

enum error_code_type
{ 
  NO_ERROR_SUCCESS              = 0x00u,
  OUT_OF_MEMORY                 = 0x01u,
  PXE_FAILED_TO_INITIALIZE      = 0x02u,
  CANT_READ_MEMORY_MAP          = 0x03u,
  ACQUIRE_BIOS_MEMMAP_FAILED    = 0x04u,
  ACQUIRE_BIOS_MEMMAP_OVERFLOW  = 0x05u
};

typedef uint16_t extended_error_code_type;

typedef uint32_t combined_error_type;

combined_error_type make_combined_error   (error_code_type, extended_error_code_type);
char const far*     error_code_to_string  (error_code_type, extended_error_code_type);
void                halt_with_error       (combined_error_type);