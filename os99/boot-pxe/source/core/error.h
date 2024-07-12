#pragma once

#include <stdint.h>
#include <stddef.h>

enum error_category_type
{ 
  NO_ERROR_SUCCESS              = 0x00u,
  OUT_OF_MEMORY                 = 0x01u,
  PXE_FAILED_TO_INITIALIZE      = 0x02u,
  CANT_READ_MEMORY_MAP          = 0x03u,
  ACQUIRE_BIOS_MEMMAP_FAILED    = 0x04u,
  ACQUIRE_BIOS_MEMMAP_OVERFLOW  = 0x05u,
  ACQUIRE_PXE_DHCP_INFO_FAILED  = 0x06u,
  NOT_ENOUGH_EXTENDED_MEMORY    = 0x07u,
  CORE_IMAGE_BAD_OR_NOT_FOUND   = 0x08u  
};

typedef uint16_t error_code_type;

typedef uint32_t error_type;

error_type          make_error                (error_category_type, error_code_type = 0);
char const far*     error_category_to_string  (error_type);
void                halt_with_error           (error_type);
error_category_type category_from_error       (error_type);
error_code_type     code_from_error           (error_type);
