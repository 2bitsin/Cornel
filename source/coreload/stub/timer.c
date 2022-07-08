#include "timer.h"
#include "irqstubs.h"
#include "print.h"
#include "serial.h"
#include "rtccmos.h"

void PIT_init()
{
  DBG_print_char('\n');
  IRQ_init(IRQ_INIT_IRQ0_BIT); 
}

uint16_t __cdecl PIT_irq(uint16_t irq_n)
{
  DBG_print_string("\rTIME = ");
  DBG_print_dec8(CMOS_read_byte(CMOS_HOURS_REG));
  DBG_print_char(':');
  DBG_print_dec8(CMOS_read_byte(CMOS_MINUTES_REG));
  DBG_print_char(':');
  DBG_print_dec8(CMOS_read_byte(CMOS_SECONDS_REG));
  DBG_print_string("   ");

  SER_sync_send_string(SERIAL_PORT_COM1, "TICK!\r\n"); 
  return IRQ_CALL_DEFAULT;
}

