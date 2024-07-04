#pragma once

#include <stdint.h>
#include <stddef.h>

enum error_code_type
{
  OUT_OF_MEMORY             = 0x00u,
  PXE_FAILED_TO_INITIALIZE  = 0x01u
};
typedef uint16_t extended_error_code_type;

void            halt_with_error       (error_code_type, extended_error_code_type);
char const far* error_code_to_string  (error_code_type, extended_error_code_type);