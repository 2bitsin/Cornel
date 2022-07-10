#include "timer.h"
#include "irqstubs.h"
#include "print.h"
#include "serial.h"
#include "rtccmos.h"

void PIT_init()
{
  #if 0
  DBG_print_char('\n');
  IRQ_init(IRQ_INIT_IRQ0_BIT); 
  #endif
}

uint16_t __cdecl PIT_irq(uint16_t irq_n)
{
  #if 0
  uint16_t base;

  DBG_print_string("\rTIME = ");
  DBG_print_hex8(CMOS_read_byte(CMOS_HOURS_REG));
  DBG_print_char(':');
  DBG_print_hex8(CMOS_read_byte(CMOS_MINUTES_REG));
  DBG_print_char(':');
  DBG_print_hex8(CMOS_read_byte(CMOS_SECONDS_REG));
  DBG_print_string("   ");

  base = SER_get_port_base(SERIAL_PORT_COM1);
  SER_sync_transmit_string(base, "TICK!\r\n"); 
  #endif
  return IRQ_CALL_DEFAULT;  
}

