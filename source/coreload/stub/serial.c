#include "serial.h"
#include "x86asm.h"
#include "irqstubs.h"
#include "print.h"

void __cdecl SER_irq(int16_t irq_n)
{
}

void SER_init()
{ 
  uint16_t value;
  uint16_t irq_mask;
  
  irq_mask = 0;

  DBG_print_string("\nInitializing serial COM's ...... ");                  
  DBG_print_string("\n* Base addresses:");
    
  if (value = read_bda_word(0)) {
    DBG_print_string("\n  COM1: ");
    DBG_print_hex16(value);
    irq_mask |= IRQ_INIT_IRQ4_BIT;
  }
  if (value = read_bda_word(2)) {
    DBG_print_string("\n  COM2: ");
    DBG_print_hex16(value);
    irq_mask |= IRQ_INIT_IRQ3_BIT;
  }
  if (value = read_bda_word(4)) {
    DBG_print_string("\n  COM3: ");
    DBG_print_hex16(value);
    irq_mask |= IRQ_INIT_IRQ4_BIT;
  }
  if (value = read_bda_word(6)) {
    DBG_print_string("\n  COM4: ");
    DBG_print_hex16(value);
    irq_mask |= IRQ_INIT_IRQ3_BIT;
  }
  IRQ_init(irq_mask);

  DBG_print_char('\n');  
}

void SER_setup_port(int16_t port)
{

}
