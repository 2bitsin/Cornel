
#include "types.h"
#include "flatreal.h"
#include "x86asm.h"
#include "kbdctrl.h"
#include "rtccmos.h"
#include "print.h"

#define FAST_GATE_A20_ENABLE_BIT 0x02
#define FAST_GATE_RESET_BIT      0x01

int16_t A20_is_disabled()
{ 
  static uint32_t value = 0;
  uint16_t flags;

  flags = x86_flags16();
  x86_cli();
  FLAT_fill4(0x000500, 1, 0xAA55AA55);  
  FLAT_fill4(0x100500, 1, 0x55AA55AA);
  FLAT_copy4(FLAT_neartolinear(&value), 0x000500, 1);
  x86_load_flags16(flags);

  // Gate enabled
  if(value == 0xAA55AA55)
    return 0;

  // Gate disabled  
  if(value == 0x55AA55AA)
    return 1;

  DBG_print_string("\n#0004 - A20 gate test disturbed, memory failure ?");
  return -1;
}

static inline void A20_fast_enable()
{
  uint8_t state = x86_inb(0x92);
  if (!(state & FAST_GATE_A20_ENABLE_BIT)) 
  {
    state &= ~FAST_GATE_RESET_BIT;    
    state |= FAST_GATE_A20_ENABLE_BIT;
    x86_outb(0x92, state);
  }
}

static inline void A20_port_ee_enable()
{
  x86_inb(0xee);
}

static inline void A20_kbc_enable()
{
  uint8_t status;
  uint16_t flags;

  flags = x86_flags16();
  x86_cli();
  KBC_disable_keyboard();
  status = KBC_read_output_port();
  status |= KBC_OUTPUT_PORT_A20_ENABLE_BIT;
  KBC_send_output_port(status); 
  KBC_enable_keyboard();
  x86_load_flags16(flags);
}

uint16_t A20_enable_bios_15h();
#pragma aux A20_enable_bios_15h = \
  "stc"             \
  "mov ax, 0x2401"  \
  "int 0x15"        \
  "mov al, ah"      \
  "lahf"            \
  "and ah, 1"       \
  value [ax]        \
  
int16_t A20_init()
{
  DBG_print_string("\nTaking care of A20 gate ........ ");
  if (!A20_is_disabled()) {
    DBG_print_string("already enabled.");
    return 0;  
  }
  DBG_print_string("not enabled.");  
  DBG_print_string("\n * Trying port 0xEE ............ ");
  A20_port_ee_enable();
  if (!A20_is_disabled()) {
    DBG_print_string("success.");
    return 0;
  }   
  DBG_print_string("failed.");
  DBG_print_string("\n * Trying fast gate A20 ........ ");
  A20_fast_enable();
  if (!A20_is_disabled()) {
    DBG_print_string("success.");
    return 0;
  }   
  DBG_print_string("failed.");
  DBG_print_string("\n * Trying PS/2 controller ...... ");
  A20_kbc_enable();
  if (!A20_is_disabled()) {
    DBG_print_string("success.");
    return 0;
  }
  DBG_print_string("failed.");
  DBG_print_string("\n * Trying int 15H, AH=0x2401 ... ");
  A20_enable_bios_15h();
  if (!A20_is_disabled()) {
    DBG_print_string("success.");
    return 0;
  }
  DBG_print_string("failed.");
  print_string("\n#0005 - failed to enable A20 gate.");
  return 1;
}
