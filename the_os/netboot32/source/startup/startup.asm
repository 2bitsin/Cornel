    format binary    
    use16

    org     0x7c00
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
  .end:

    dw 0x0000 

gdtr_bits:
    dw desctbl.end - desctbl.begin
    dd desctbl.begin

    align   16

    include "nmictl.asi"
    include "print.asi"

preamble:
    pop     eax             ; Discard return address
    pop     eax             ; Grab pointer to !PXE structure
    
    movzx   edx,    ax      ; edx = offset of !PXE structure
    shr     eax,    16      ; eax = segment of !PXE structure
    shl     eax,    4       ; eax = segment of !PXE structure * 16
    add     eax,    edx     ; eax = linear address of !PXE structure
    push    eax             ; Save pointer to !PXE structure

    mov     ax,     es      ; ax = segment of PXENV+ structure
    movzx   eax,    ax      ; eax = segment PXENV+ structure
    shl     eax,    4       ; eax = PXENV+ structure * 16
    movzx   ebx,    bx      ; ebx = offset PXENV+ structure
    add     eax,    ebx     ; eax = linear address PXENV+ structure
    push    eax             ; save PXENV+ structure address on stack   

    mov     ax,     cs
    mov     ds,     ax
    mov     es,     ax
    mov     fs,     ax
    mov     gs,     ax

    mov     si,     strings.hello
    call    putstr    

pmode_start:

    lgdt    [gdtr_bits]

    cli     
    call    non_maskable_irq_off
    
    mov     eax,    cr0
    or      eax,    1
    mov     cr0,    eax

    jmp     0x0008:$+5

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

    include "reboot.asi"

    times ((($ + 0xf) and not 0xf) - $) nop

start_32bit_code: