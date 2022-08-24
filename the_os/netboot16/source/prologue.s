.386
.model tiny

  extern STUB_main_:proc near

_PROLOGUE segment use16 public 'PROLOGUE'

  _entry proc near public
    mov     cx,     es
    mov     dx,     bx
    add     sp,     4
    mov     bp,     sp
    mov     bx,     word ptr [bp]
    mov     ax,     word ptr [bp + 2]
    push    G_BASE_ADDRESS/16    
    db      0x68
    dw      offset STUB_main_
    retf

  _entry endp

_PROLOGUE ends

_STACK segment use16 public 'STACK'
_STACK ends

end

