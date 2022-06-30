.386
.model tiny

  extern bootstrap_:proc near

_TEXT segment use16 public 'CODE'

_bootstrap proc near public 

  db 0xea
  dw offset _bootstrap_adjust_segment
  dw (G_LOAD_ADDRESS - G_STACK_SIZE)/16

_bootstrap_adjust_segment:

  mov         sp,     G_STACK_SIZE
  mov         ax,     cs
  mov         ss,     ax
  mov         ds,     ax
  mov         es,     ax
  mov         fs,     ax
  mov         gs,     ax
  
  call        bootstrap_

_bootstrap_wait_8042:

  in          al,     0x64
  test        al,     0x02
  jnz         _bootstrap_wait_8042
  mov         al,     0xfe
  out         0x64,   al

  cli
  hlt

_bootstrap endp


_TEXT ends

end

