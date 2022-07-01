.386
.model tiny

  extern bootstrap_:proc near

_TEXT segment use16 public 'CODE'

_startup proc near public 

  db 0xea
  dw offset _startup_adjust_segment
  dw (G_LOAD_ADDRESS - G_STACK_SIZE)/16

_startup_adjust_segment:

  mov         sp,     G_STACK_SIZE
  mov         ax,     cs
  mov         ss,     ax
  mov         ds,     ax
  mov         es,     ax
  mov         fs,     ax
  mov         gs,     ax
  
  call        bootstrap_

_startup_wait_8042:

  in          al,     0x64
  test        al,     0x02
  jnz         _startup_wait_8042
  mov         al,     0xfe
  out         0x64,   al

  cli
  hlt

_startup endp


_TEXT ends

end

