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

