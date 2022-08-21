    format binary    
    use16

    org     0x7c00
    jmp     preamble
    times   (16 - $ mod 16) nop

disptbl:
    dd      putstr
    dd      putstr
    dd      putstr
    dd      putstr    


preamble:
    mov     ax,     cs
    mov     ds,     ax
    mov     es,     ax
    mov     fs,     ax
    mov     gs,     ax

    mov     si,     hello
    call    putstr
    
    cli
    hlt



include "print.s"

desctbl:
    dq 0x0000000000000000 ; 0x0000 = null
    dq 0x00cf9e000000ffff ; 0x0008 = 32bit code 
    dq 0x00cf92000000ffff ; 0x0010 = 32bit data/stack
    dq 0x00009e000000ffff ; 0x0018 = 16bit code
    dq 0x000092000000ffff ; 0x0020 = 16bit data/stack

hello:
    db      "Hello, world!",13,10,0

    times   (512 - $ mod 512) nop

start_32: