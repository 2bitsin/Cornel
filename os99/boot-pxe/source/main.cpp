#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <malloc.h>

#include "core/error.h"
#include "x86/asm.h"
#include "pxe/pxe.h"

int main(int, char**) 
{     
  uint16_t v_status = 0;  
  puts("BOOT-PXE v0.1 (BUILD: " __TIME__ " " __DATE__ ")");
  v_status = PXE_init();
  if (v_status != success) {
    halt_with_error(PXE_FAILED_TO_INITIALIZE, v_status);
    return -1;
  }
  PXE_print_info();
      
  return 0;
}


