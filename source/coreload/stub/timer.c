#include "timer.h"
#include "irqstubs.h"

void PIT_init()
{
  IRQ_init(IRQ_INIT_IRQ0_BIT);
}

void __cdecl PIT_irq(uint16_t irq_n)
{}

