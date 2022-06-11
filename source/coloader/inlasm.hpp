#ifndef __INLASM_HPP__
#define __INLASM_HPP__

#include "types.hpp"

void io_write_b(uword addr, ubyte val);
#pragma aux io_write_b =          \
  "out dx, al"                    \
  parm [dx] [al]                  ;

void io_write_w(uword addr, uword val);
#pragma aux io_write_w =          \
  "out dx, ax"                    \
  parm [dx] [ax]                  ;

void io_write_d(uword addr, udword val);
#pragma aux io_write_d =          \
  "shl eax, 16"                   \
  "mov ax, cx"                    \
  "out dx, eax"                   \
  parm [dx] [ax cx]               ;

ubyte io_read_b(uword addr);
#pragma aux io_read_b =           \
  "in al, dx"                     \
  parm [dx]                       \
  value [al]                      ;

uword io_read_w(uword addr);
#pragma aux io_read_w =           \
  "in ax, dx"                     \
  parm [dx]                       \
  value [ax]                      ;

udword io_read_d(uword addr);
#pragma aux io_read_d =           \
  "in eax, dx"                    \
  "mov dx, ax"                    \
  "shr eax, 16"                   \
  parm [dx]                       \
  value [ax dx]                   ;

void irq_disable();
#pragma aux irq_disable =         \
  "cli"                           ;
  
void irq_enable();
#pragma aux irq_enable =          \
  "sti"                           ;

void irq_wait();
#pragma aux irq_wait =            \
  "hlt"                           ;


#endif