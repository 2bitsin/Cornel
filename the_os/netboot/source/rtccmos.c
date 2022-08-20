#include "rtccmos.h"
#include "x86asm.h"

#define NMI_DISABLE_BIT 0x80

void RTC_nmi_disable()
{
  x86_outb (0x70, x86_inb(0x70) | NMI_DISABLE_BIT);
}

void RTC_nmi_enable()
{
  x86_outb (0x70, x86_inb(0x70) & ~NMI_DISABLE_BIT);
}

uint8_t CMOS_read_byte(uint8_t addr)
{
  uint8_t nmi_status, value;  
  nmi_status = x86_inb(0x70) & NMI_DISABLE_BIT;
  x86_cli();
  x86_outb(0x70, addr | NMI_DISABLE_BIT);
  value = x86_inb(0x71);
  x86_outb(0x70, nmi_status);
  return value;
}

