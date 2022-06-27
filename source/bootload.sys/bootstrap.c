#include "x86assembly.h"
#include "memquery.h"
#include "memmap.h"
#include "print.h"
#include "keyboard.h"

void bootstrap ()
{     
  print_string("Starting bootload.sys\n");
  memmap_e820();
  print_string("Boot failed.\nPress any key to reboot.\n");
  wait_for_key();
}

