#include "x86/flatmem.h"


void __declspec(naked) flat_fill(
  uint32_t  target_address, 
  uint8_t   fill_with, 
  uint32_t  bytes_to_fill) 
{__asm{  
  push      bp
  mov       bp,     sp
  pushf
  push      edi
  xor       ax,     ax
  mov       es,     ax
  mov       edi,    [bp+4]
  mov       al,     [bp+8]
  mov       ecx,    [bp+10]
  cld
  db        0x67
  rep       stosb
  pop       edi
  popf
  pop       bp
  ret
}}


void __declspec(naked) flat_copy(
  uint32_t  target_address, 
  uint32_t  source_address, 
  uint32_t  bytes_to_copy)
{__asm{
  push      bp
  mov       bp,     sp
  pushf
  push      ds
  push      edi
  push      esi
  xor       cx,     cx
  mov       ds,     cx
  mov       es,     cx
  mov       ecx,    [bp+12]
  mov       esi,    [bp+8]
  mov       edi,    [bp+4]
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
  pop       esi
  pop       edi
  pop       ds
  popf
  pop       bp  
  ret
}} 

