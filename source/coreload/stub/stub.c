#include "types.h"
#include "x86asm.h"
#include "memory.h"
#include "print.h"
#include "bioskey.h"
#include "kbdctrl.h"
#include "flatreal.h"
#include "atwenty.h"
#include "irqstubs.h"
#include "serial.h"
#include "timer.h"

static const serial_port_init_type G_com1_init = {
  .baud       = 9600,
  .data_bits  = 8,
  .parity     = SERIAL_PARITY_NONE,
  .stop_bits  = 1
};

int16_t STUB_init ()
{     
  int16_t status;
  uint16_t com_base;

  DBG_print_string("Cornel loader v0.1 : \n"); 
  DBG_print_string("Initializing...\n");
  DBG_print_char('\n');
  FLAT_init();   
  DBG_print_char('\n');
  A20_init();
  DBG_print_char('\n');
  MEM_init();
  DBG_print_char('\n');
  SER_init();

  com_base = SER_init_port(SERIAL_PORT_COM1, &G_com1_init);
  if (com_base == 0)
  {
    print_string("#0006 - Error initializing COM1!\n");
    return -1;
  }
  SER_sync_transmit_string(com_base, "Cornel loader v0.1 : \n");
  PIT_init();
  x86_sti();
  for(;;) {
    DBG_print_char (SER_sync_receive_byte(com_base));
    //x86_hlt();
  }
  return 0;
}

void STUB_exit () 
{
  print_string("\nPress any key to reboot...\n");
  wait_for_key();  
}

__declspec(noreturn)
void STUB_main ()
{ 
  // Initialize STACK    
  x86_load_ss_sp(G_BASE_ADDRESS/16, G_STACK_SIZE);
  x86_load_all_seg(x86_cs());

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
