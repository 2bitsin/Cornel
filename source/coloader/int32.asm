.386
.model tiny


_TEXT segment 'CODE' para public USE16

__I4D proc public

  push  dx
  push  ax  
  pop   eax
  xor   edx, edx

  push  cx
  push  bx
  pop   ebx

  idiv  ebx

  push  edx ; << -- remainder
  push  eax ; << -- quotient

  pop   ax
  pop   dx
  pop   bx
  pop   cx

  ret

__I4D endp

__U4D proc public

  push  dx
  push  ax  
  pop   eax
  xor   edx, edx

  push  cx
  push  bx
  pop   ebx

  div  ebx

  push  edx ; << -- remainder
  push  eax ; << -- quotient

  pop   ax
  pop   dx
  pop   bx
  pop   cx

  ret

__U4D endp

_TEXT ends

end