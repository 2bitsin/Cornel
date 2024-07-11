#include <stdio.h>

#include <string>
#include <iostream>

#include "core/error.h"
#include "core/sysinfo.h"
#include "pxe/pxe.h"
#include "x86/debug.h"
#include "x86/flatmem.h"

static const char test[] = "Hello World!";

int main(int, char**) 
{     
  uint16_t v_status = 0;    
  puts("BOOT-PXE v0.1 (BUILD: " __TIME__ " " __DATE__ ")");
  v_status = PXE_init();
  if (v_status != success)
    halt_with_error(make_error(PXE_FAILED_TO_INITIALIZE, v_status));
  v_status = SYSINFO_acquire();
  if (v_status != NO_ERROR_SUCCESS)
    halt_with_error(v_status);
  return 0;
}


