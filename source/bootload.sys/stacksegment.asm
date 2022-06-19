_STACK segment use16 at 0x0000 public 'STACK'
  
  DB 0x7c00 DUP(?)

_G_stack_top proc far public
_G_stack_top endp

_STACK ends

end