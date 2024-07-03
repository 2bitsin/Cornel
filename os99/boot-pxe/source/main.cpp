#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>

#include "x86/asm.h"
#include "pxe/pxe.h"


int main(int, char**) 
{       
  puts("BOOT-PXE v0.1 (BUILD: " __TIME__ " " __DATE__ ")");
  PXE_init();
  return 0;
}

