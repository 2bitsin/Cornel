
#include "types.h"
#include "flatreal.h"
#include "x86assembly.h"
#include "kbdctrl.h"
#include "rtccmos.h"

#define FAST_GATE_A20_ENABLE_BIT 0x02
#define FAST_GATE_RESET_BIT      0x01

int16_t A20_is_disabled()
{ 
  static uint32_t value = 0;
  x86_cli();
  RTC_nmi_disable();
  FLAT_fill4(0x000500, 1, 0xAA55AA55);  
  FLAT_fill4(0x100500, 1, 0x55AA55AA);
  FLAT_copy4(FLAT_neartolinear(&value), 0x000500, 1);
  RTC_nmi_enable();
  x86_sti();

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

static void A20_kbc_enable()
{
 
}

int16_t A20_init()
{
  DBG_print_string("\nTaking care of A20 gate ... ");
  if (!A20_check()) {
    DBG_print_string("already enabled.");
    return 0;  
  }

  DBG_print_string("\n * Trying port 0xee ....... ");
  A20_port_ee_enable();
  if (!A20_check()) {
    DBG_print_string("success.\n");
    return 0;
  }   

  DBG_print_string("\n * Trying fast gate a20 ... ");
  A20_fast_enable();
  if (!A20_check()) {
    DBG_print_string("success.\n");
    return 0;
  }   

  DBG_print_string("\n * Trying port 0x92 ....... ");

}
