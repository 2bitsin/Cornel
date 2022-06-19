#include "x86assembly.h"

extern int G_stack_top;

__declspec(noreturn) 
void bootload ()
{
  x86_load_ss_sp(&G_stack_top);


  x86_cli();
  x86_hlt();
}

