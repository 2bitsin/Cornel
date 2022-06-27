.386
.model tiny

_STACK segment use16 public 'STACK'

_G_stack_body proc near public
  db 0x4000 dup (1)
_G_stack_body endp

_G_stack_top proc near public
_G_stack_top endp

_STACK ends

end