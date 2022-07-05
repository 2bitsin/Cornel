#include "types.h"
#include "x86assembly.h"
#include "memory.h"
#include "print.h"
#include "bioskey.h"
#include "kbdctrl.h"
#include "flatreal.h"
#include "atwenty.h"

int16_t STUB_init ()
{     
  int16_t status;

  DBG_print_string("Starting Cornel OS:\n");    
  FLAT_init();   
  A20_init();
  MEM_init();
  return 0;
}

void STUB_exit () 
{
  print_string("Press any key to reboot...\n");
  wait_for_key();  
}

__declspec(noreturn)
void STUB_main ()
{   
  x86_cli();
  x86_load_sp(G_STACK_SIZE);
  x86_load_all_seg(x86_cs());
  x86_sti();
  STUB_init();
  STUB_exit();
  KBC_hardware_reset();
  x86_cli();
  x86_hlt();
}
