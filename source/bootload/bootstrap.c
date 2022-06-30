#include "x86assembly.h"
#include "memory.h"
#include "print.h"
#include "keyboard.h"

void bootstrap ()
{     
  print_string("Starting bootload.sys\n");
  MEM_populate((void*)0);
  print_string("Boot failed.\nPress any key to reboot.\n");
  wait_for_key();
}

