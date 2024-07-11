#include "x86/flatmem.h"

uint32_t __declspec(naked) flat_fill(
  uint32_t  target_address, 
  uint8_t   fill_with, 
  uint32_t  bytes_to_fill) 
{__asm{  
  pushad

  popad
  ret
}}


uint32_t __declspec(naked) flat_copy(
  uint32_t  target_address, 
  uint32_t  source_address, 
  uint32_t  bytes_to_copy)
{__asm{
  pushad
  mov       bp,     sp
  pushf
  push      ds
  push      es
  xor       cx,     cx
  mov       ds,     cx
  mov       es,     cx
  mov       ecx,    [bp+42]
  mov       esi,    [bp+38]
  mov       edi,    [bp+34]
  cmp       edi,    esi
  jz        _done_
  ja        _bkwd_
  cld
  db        0x67
  rep       movsb  
  jmp       _done_
_bkwd_:
  std
  add       esi,    ecx
  dec       esi
  add       edi,    ecx
  dec       edi
  db        0x67
  rep       movsb
_done_:
  popf
  pop       es
  pop       ds
  popad
  ret
}} 

