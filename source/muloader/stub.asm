    
    org 0x0000

    use16

  start_here:

    jmp       0x7c0:start
  start:
  ; setup segments
    mov       ax,         0x7c0 
    mov       ds,         ax
    mov       es,         ax
    mov       fs,         ax
    mov       gs,         ax
    cld 

  ; setup stack  
    mov       ss,         ax
    mov       sp,         0x7c00

  ; clear PSP
    mov       cx,         0x80
    mov       di,         psp
    xor       ax,         ax
    rep       stosw

  ; setup dummy PSP
    
    mov       di,         psp
    mov       ax,         0x20cd
    stosw
    mov       ax,         0xa000
    stosw
    mov       di,         psp + 0xa
    mov       eax,        0x07c00000 + int20
    stosd
    stosd
    stosd
    mov       ax,         0x07c0
    stosw   

  ; setup interrupt vectors

    push      es
    xor       di,         di
    mov       es,         di
    mov       di,         0x20 * 4

    mov       eax,        0x07c00000 + int20
    stosd
    mov       eax,        0x07c00000 + int21
    stosd
    mov       eax,        0x07c00000 + int22
    stosd
    mov       eax,        0x07c00000 + int23
    stosd
    mov       eax,        0x07c00000 + int24
    stosd
    mov       eax,        0x07c00000 + int25
    stosd
    mov       eax,        0x07c00000 + int26
    stosd
    mov       eax,        0x07c00000 + int27
    stosd
    mov       eax,        0x07c00000 + int28
    stosd
    mov       eax,        0x07c00000 + int29
    stosd
    mov       eax,        0x07c00000 + int2a
    stosd
    mov       eax,        0x07c00000 + int2b
    stosd
    mov       eax,        0x07c00000 + int2c
    stosd
    mov       eax,        0x07c00000 + int2d
    stosd
    mov       eax,        0x07c00000 + int2e
    stosd
    mov       eax,        0x07c00000 + int2f
    stosd

    pop       es  

    xor       ax,         ax
    xor       bx,         bx
    mov       cx,         0xff
    mov       dx,         0x800
    mov       es,         dx
    mov       ds,         dx
    mov       ss,         dx
    mov       si,         0x100
    mov       di,         0xfffe
    mov       bp,         0x900
    mov       sp,         0x0000
    
    jmp       0x800:0x0100

  DOS_resize_memory_block:
    push      es
    pop       ax
    cmp       ax,         0x800
    je        .l0
    mov       ax,         0x9
    stc
    iret

  .l0:
    cmp       bx,         0xa000 - 0x800
    ja        .l1
    xor       ax,         ax
    clc
    iret

  .l1:
    mov       bx,         0xa000 - 0x800 ; maximum available
    mov       ax,         0x8 ; insufficient memory
    stc
    iret
; ----------------------------------------------
  int20:
    push      0x20
    pop       fs
    cli 
    hlt
  int21:
    push      0x21
    pop       fs
    cmp       ah,         0x4a
    je        DOS_resize_memory_block
     
    cli 
    hlt
  int22:
    push      0x22
    pop       fs
    cli 
    hlt
  int23:
    push      0x23
    pop       fs
    cli 
    hlt
  int24:
    push      0x24
    pop       fs
    cli 
    hlt
  int25:
    push      0x25
    pop       fs
    cli 
    hlt
  int26:   
    push      0x26
    pop       fs
    cli 
    hlt
  int27:   
    push      0x27
    pop       fs
    cli 
    hlt
  int28:   
    push      0x28
    pop       fs
    cli 
    hlt
  int29:   
    push      0x29
    pop       fs
    cli 
    hlt
  int2a:   
    push      0x2a
    pop       fs
    cli 
    hlt
  int2b:   
    push      0x2b
    pop       fs
    cli 
    hlt
  int2c:   
    push      0x2c
    pop       fs
    cli 
    hlt
  int2d:   
    push      0x2d
    pop       fs
    cli 
    hlt
  int2e:   
    push      0x2e
    pop       fs
    cli 
    hlt
  int2f:   
    push      0x2f
    pop       fs
    cli 
    hlt
    
    times     1024 - ($ - $$) db 0

  psp:
    times     256 db 0



