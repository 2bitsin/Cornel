    format binary    
    use16

    LOAD_ADDRESS equ 0x7c00

    org     LOAD_ADDRESS
    jmp     preamble    

strings:
  .hello:
    db      "NETBOOT32 v0.1 BUILD ", G_BUILD_TIMESTAMP, 13,10,0

    align   16

desctbl:

  .begin:
    dq 0x0000000000000000 ; 0x0000 = null
    dq 0x00cf9e000000ffff ; 0x0008 = 32bit code 
    dq 0x00cf92000000ffff ; 0x0010 = 32bit data/stack
    dq 0x00009e000000ffff ; 0x0018 = 16bit code
    dq 0x000092000000ffff ; 0x0020 = 16bit data/stack
  .end:

    dw 0x0000 

gdtr_bits:
    dw desctbl.end - desctbl.begin
    dd desctbl.begin

    align   16

    include "nmictl.asi"
    include "console.asi"

preamble:

    xor     sp,     sp
    mov     ss,     sp
    mov     sp,     LOAD_ADDRESS  ; Reset stack pointer

    mov     ax,     cs            ; ax = segment of PXENV+ structure
    mov     ds,     ax            ; 
    mov     es,     ax            ; 
    mov     fs,     ax            ; 
    mov     gs,     ax            ; 

    call    coninit
    mov     si,     strings.hello
    call    putstr    

pmode_start:

    lgdt    [gdtr_bits]

    cli     
    call    non_maskable_irq_off
    
    mov     eax,    cr0
    or      eax,    1
    mov     cr0,    eax

    jmp     0x0008:.32
.32:
use32

    mov     ax,     ss
    movzx   eax,    ax
    shl     eax,    4
    add     esp,    eax

    mov     ax,     0x10
    mov     ss,     ax
    mov     ds,     ax
    mov     es,     ax
    mov     fs,     ax
    mov     gs,     ax    

    call    start_32bit_code
    call    reboot_via_8042    
    cli
    hlt    

    include "reboot.asi"

    times ((($ + 0xf) and not 0xf) - $) nop

start_32bit_code: