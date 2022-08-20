#include "timer.h"
#include "irqctrl.h"
#include "print.h"
#include "serial.h"
#include "rtccmos.h"
#include "flatreal.h"

void PIT_init()
{  
  IRQ_init(IRQ_INIT_IRQ0_BIT);   
  IRQ_unmask(IRQ_INIT_IRQ0_BIT);
}

uint16_t __cdecl PIT_irq(uint16_t irq_n)
{  
  static const char hex_chars[] = "0123456789ABCDEF";
  static const char cprogress[] = "\xfa\xfa\xfa\xfa\xf9\xf9\xf9\xf9\x03\x03\x03\x03\x03\x03\x03\x03";
  static uint8_t cprogress_index = 0;

  uint16_t base, hour, min, sec, i;

  hour = CMOS_read_byte(CMOS_HOURS_REG);
  min  = CMOS_read_byte(CMOS_MINUTES_REG);
  sec  = CMOS_read_byte(CMOS_SECONDS_REG);

  FLAT_fill2(0xB8000, 80, 0x0720);
  i  = 0; FLAT_fill2(0xB8000 + i, 1, 0x700 | 0x0700 | hex_chars[hour >> 4]);  
  i += 2; FLAT_fill2(0xB8000 + i, 1, 0x700 | hex_chars[hour & 0xf]);
  i += 2; FLAT_fill2(0xB8000 + i, 1, 0x700 | ':');
  i += 2; FLAT_fill2(0xB8000 + i, 1, 0x700 | hex_chars[min >> 4]);
  i += 2; FLAT_fill2(0xB8000 + i, 1, 0x700 | hex_chars[min & 0xf]);
  i += 2; FLAT_fill2(0xB8000 + i, 1, 0x700 | ':');
  i += 2; FLAT_fill2(0xB8000 + i, 1, 0x700 | hex_chars[sec >> 4]);
  i += 2; FLAT_fill2(0xB8000 + i, 1, 0x700 | hex_chars[sec & 0xf]);
  i += 2; FLAT_fill2(0xB8000 + i, 1, 0x700 | ' ');
  i += 2; FLAT_fill2(0xB8000 + i, 1, 0x700 | cprogress[cprogress_index++ % (sizeof(cprogress)-1)]);
     
  return IRQ_CALL_DEFAULT;  
}

