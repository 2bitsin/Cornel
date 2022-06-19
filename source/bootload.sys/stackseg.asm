_STACK segment use16 at 0x0050 public 'STACK'
  
  DB 0x7700 DUP(?)

_G_stack_top proc far public
_G_stack_top endp

_STACK ends

end