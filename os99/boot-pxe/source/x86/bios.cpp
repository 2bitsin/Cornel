#include <i86.h>
#include "x86/bios.h"


uint16_t BIOS_query_memmap(memmap_entry 
  far* v_buff, 
  uint32_t *v_size, 
  uint32_t* v_next)
{
//  REGPACK r;
//  r.x.eax = 0xe820;
//  r.x.edx = 0x534d4150;
//  r.x.ebx = v_next?*v_next:0;
//  r.x.ecx = sizeof(*v_buff);
//  r.w.di = FP_OFF(v_buff);
//  r.w.es = FP_SEG(v_buff);
//  intr(0x15, &r);
//  if (r.w.flags & INTR_CF || r.x.eax != 0x534d4150)
//    return r.w.ax;
//  *v_size = r.x.ecx;
//  *v_next = r.x.ebx;


  return 0;
}