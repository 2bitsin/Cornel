#include <stdint.h>
#include <stddef.h>
#include <i86.h>

#include "doshelp.h"
#include "asm.h"
#include "intr.h"

void __watcall __far dos_fun_44(int_regs_t far* r) 
{
  if (r->al != 0)
  {
    __debugbreak();
    r->ax = 0x1;
    r->flags |= 0x0001;
    return;
  }
  switch(r->bx)
  {
  case 1:
    r->dx = r->ax = 0x0082;
    r->flags &= 0xfffe;
    return;
  case 2:
    r->dx = r->ax = 0x0081;
    r->flags &= 0xfffe;
    return;
  default:
    r->flags |= 0x0001;
    r->ax = 0x6;
    return;
  }
}

void __watcall __far dos_fun_40(int_regs_t far* r) 
{    
  union REGS nr;
  size_t i = 0;  
  if (r->bx == 1) {
    for(i = 0; i < r->cx; ++i) 
    {
      nr.h.al = *(r->ds:>(char*)(r->dx + i));
      nr.h.ah = 0xe;
      nr.w.bx = 0;
      int86(0x10, &nr, &nr);
    }
    r->flags &= 0xfffe;
    r->ax = r->cx;   
  } else {
    r->ax = 0x6;
    r->flags |= 0x0001;
  }
}

void __watcall __far dos_fun_63(int_regs_t far* r) 
{
  static uint8_t __far tbl [2] = { 0, 0 };
  r->flags &= 0xfffe;
  r->al = 0;
  r->ds = FP_SEG(tbl);
  r->si = FP_OFF(tbl);
}

void init_dos_helper()
{
  pfvoid_t far* v_table = (pfvoid_t far*)get_ivt_entry(0xff);
  v_table[0x40] = &dos_fun_40;
  v_table[0x44] = &dos_fun_44;
  v_table[0x63] = &dos_fun_63;
}