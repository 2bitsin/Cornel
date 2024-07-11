#include "core/error.h"
#include <stdio.h>
#include <stdlib.h>

char const far *error_code_to_string(error_type error_value)
{

#define X(y, x) \
  case x:       \
    return #x
  switch (category_from_error(error_value))
  {
    X(NO_ERROR_SUCCESS, 0x00u);
    X(OUT_OF_MEMORY, 0x01u);
    X(PXE_FAILED_TO_INITIALIZE, 0x02u);
    X(CANT_READ_MEMORY_MAP, 0x03u);
    X(ACQUIRE_BIOS_MEMMAP_FAILED, 0x04u);
    X(ACQUIRE_BIOS_MEMMAP_OVERFLOW, 0x05u);
    X(ACQUIRE_PXE_DHCP_INFO_FAILED, 0x06u);
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