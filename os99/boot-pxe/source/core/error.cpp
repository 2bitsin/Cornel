#include "core/error.h"
#include <stdio.h>
#include <stdlib.h>

char const far *error_code_to_string(error_type error_value)
{

  switch (category_from_error(error_value))
  {
#define X(y, x) \
  case x:       \
    return #x
    X(0x00u, NO_ERROR_SUCCESS);
    X(0x01u, OUT_OF_MEMORY);
    X(0x02u, PXE_FAILED_TO_INITIALIZE);
    X(0x03u, CANT_READ_MEMORY_MAP);
    X(0x04u, ACQUIRE_BIOS_MEMMAP_FAILED);
    X(0x05u, ACQUIRE_BIOS_MEMMAP_OVERFLOW);
    X(0x06u, ACQUIRE_PXE_DHCP_INFO_FAILED);
    X(0x07u, NOT_ENOUGH_EXTENDED_MEMORY);
    X(0x08u, CORE_IMAGE_BAD_OR_NOT_FOUND);
    X(0x09u, PXE_DOWNLOAD_FAILED);
  default:
    return "UNDEFINED_ERROR_CODE";
  }
#undef X
}

error_type make_error(error_category_type category, error_code_type code)
{
  return (error_type)((((uint32_t)category) << 16u) + code);
}

error_category_type category_from_error(error_type error_value)
{
  return (error_category_type)(error_value >> 16u);
}
error_code_type code_from_error(error_type error_value)
{
  return (error_code_type)(error_value & 0xFFFFu);
}

void halt_with_error(error_type error_value)
{
  error_category_type category = category_from_error(error_value);
  error_code_type err_code = code_from_error(error_value);
  printf("BOOT FAILED : %04X-%04X %s\n", category, err_code,
         error_code_to_string(error_value));
  exit(-1);
}