#include "types.h"
#include "x86asm.h"
#include "memory.h"
#include "print.h"
#include "bioskey.h"
#include "kbdctrl.h"
#include "flatreal.h"
#include "atwenty.h"

void FLAT_test()
{
  FLAT_fill4(0x100000, 0x100, 0x12345678);
  FLAT_fill2(0x100400, 0x200, 0x1234);
  FLAT_fill1(0x100800, 0x400, 0x12);

  FLAT_copy4(0x200000, 0x100000, 0x100);
  FLAT_copy2(0x200400, 0x100400, 0x200);
  FLAT_copy1(0x200800, 0x100800, 0x400);
}

int16_t STUB_init ()
{     
  int16_t status;
  DBG_print_string("Starting Cornel OS:\n");    
  FLAT_init();   
  DBG_print_char('\n');

  FLAT_test();

  A20_init();
  DBG_print_char('\n');
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
  // Initialize STACK  
  x86_cli();
  x86_load_sp(G_STACK_SIZE);
  x86_load_all_seg(x86_cs());
  x86_sti();

  // Initialize STUB
  STUB_init();

  // Shutdown STUB
  STUB_exit();

  // Hardware reset
  KBC_hardware_reset();

  // If that does not happen, halt
  x86_cli();
  x86_hlt();
}
