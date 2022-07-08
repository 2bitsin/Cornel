#include "timer.h"
#include "irqstubs.h"
#include "print.h"
#include "serial.h"

void PIT_init()
{
  IRQ_init(IRQ_INIT_IRQ0_BIT); 
}

uint16_t __cdecl PIT_irq(uint16_t irq_n)
{
  DBG_print_string("\nSending trough COM1 ... ");
  SER_sync_send_string(SERIAL_PORT_COM1, "TICK!\r\n");
  DBG_print_string("done");
  return IRQ_CALL_DEFAULT;
}

