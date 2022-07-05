.386
.model tiny

  extern STUB_main_:proc near

_PROLOGUE segment use16 public 'PROLOGUE'

  _entry proc near public
  IF DEBUG_STARTUP equ 1
  Q$0:
    cli
    mov         sp,     0x9000
    mov         ss,     sp
    mov         sp,     0x0000
    call        Q$1
  Q$1:          
    pop         ax
    mov         dx,     cs
    shl         dx,     4
    add         ax,     dx
    cmp         ax,     G_LOAD_ADDRESS + (offset Q$1 - offset Q$0)
    jz          Q$2
    mov         ax,     0x0e01
    mov         bx,     0x0007
    int         0x10    
    hlt    
  Q$2:
    sti
  ENDIF
    db 0xea
    dw offset STUB_main_
    dw G_BASE_ADDRESS/16
  _entry endp

_PROLOGUE ends

_DATA segment use16 public 'DATA'
_DATA ends

end

