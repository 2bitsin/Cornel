.386
.model tiny

_STACK segment use16 public 'STACK'
  DB 0x4000 DUP(?)

_G_stack_top proc far public
_G_stack_top endp

_STACK ends

end