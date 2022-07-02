.386
.model tiny

  extern bootstrap_:proc near

_TEXT segment use16 public 'CODE'

_prologue_start proc near

L$1:

  mov         sp,     G_STACK_SIZE
  mov         ax,     cs
  mov         ss,     ax
  mov         ds,     ax
  mov         es,     ax
  mov         fs,     ax
  mov         gs,     ax
  
  call        bootstrap_

L$2:

  in          al,     0x64
  test        al,     0x02
  jnz         L$2
  mov         al,     0xfe
  out         0x64,   al

  cli
  hlt

_prologue_start endp

_TEXT ends

_DATA segment use16 public 'DATA'
_DATA ends

_PROLOGUE segment use16 public 'PROLOGUE'

_entry proc near public 
  db 0xea
  dw offset _prologue_start
  dw (G_LOAD_ADDRESS - G_STACK_SIZE)/16
_entry endp

_PROLOGUE ends

end

