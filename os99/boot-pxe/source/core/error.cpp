#include "core/error.h"
#include <stdio.h>
#include <stdlib.h>

char const far* error_code_to_string(error_code_type error_code, extended_error_code_type extended_error_code) 
{
#define X(y, x) case x: return #x
  switch (error_code) 
  {
    X(0x00, OUT_OF_MEMORY);
    X(0x01, PXE_FAILED_TO_INITIALIZE);
    default: return "UNDEFINED_ERROR_CODE";
  }
#undef X
}

void halt_with_error(error_code_type error_code, extended_error_code_type extended_error_code)
{
  printf("BOOT FAILED : %04X-%04X %s\n", error_code, extended_error_code,
    error_code_to_string(error_code, extended_error_code));  
  exit(-1);
}