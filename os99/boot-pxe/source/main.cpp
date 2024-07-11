#include <stdio.h>

#include "core/error.h"
#include "core/sysinfo.h"
#include "pxe/pxe.h"
#include "x86/debug.h"

int main(int, char**) 
{     
  uint16_t v_status = 0;    
  puts("BOOT-PXE v0.1 (BUILD: " __TIME__ " " __DATE__ ")");
  puts("Initializing PXE...");
  __debugbreak_num(0);
  v_status = PXE_init();
  if (v_status != success)
    halt_with_error(make_error(PXE_FAILED_TO_INITIALIZE, v_status));
  puts("Acquiring system information...");
  v_status = SYSINFO_acquire();
  if (v_status != NO_ERROR_SUCCESS)
    halt_with_error(v_status);
  puts("Booting from PXE...");
  return 0;
}


