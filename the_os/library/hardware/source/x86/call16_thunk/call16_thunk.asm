    org   G_ORG
   
    use32
    jmp   short call16_thunk

  _irq_m_ : db "AL"   ;dw 0xcccc
    
  _eax_   : db "L WO" ;dd 0xcccccccc
  _ebx_   : db "RK A" ;dd 0xcccccccc
  _ecx_   : db "ND N" ;dd 0xcccccccc
  _edx_   : db "O PL" ;dd 0xcccccccc
  _esi_   : db "AY M" ;dd 0xcccccccc
  _edi_   : db "AKES" ;dd 0xcccccccc
  _ebp_   : db " JAC" ;dd 0xcccccccc

  _ds_    : db "K "   ;dw 0xcccc
  _es_    : db "A "   ;dw 0xcccc
  _fs_    : db "DU"   ;dw 0xcccc
  _gs_    : db "LL"   ;dw 0xcccc
  
  _esp_   : db " BOY" ;dd 0xcccccccc
  _ss_    : db ".."   ;dw 0xcccc
  _flags_ : db ". "   ;dw 0xcccc

  _cs_ip_ : dw call16_thunk.int_xx, 0

  _o_esp_ : db "ALL " ;dd 0xcccccccc
  _o_ss_  : db "WO"   ;dw 0xcccc
  _p_idtr : db "RK"   ;dw 0xcccc
            db " AND" ;dd 0xcccccccc
  _r_idtr : dw 0x0400
            dd 0x00000000

  call16_thunk:
    ; save registers
    pushad 
    push        es
    push        ds
    push        fs
    push        gs
    pushfd

    ; save stack
    mov         [_o_ss_ ],  ss
    mov         [_o_esp_],  esp

    ; start thunk
    jmp         0x18:.load_code16
  .load_code16: 

    use16
    mov         eax,        cr0
    and         al,         0xfe
    mov         cr0,        eax
    sidt        [cs:_p_idtr]
    lidt        [cs:_r_idtr]
    jmp         0x00:.real_mode    
  .real_mode:   
    ; ---- enable nmi --------------
    in          al,         0x70
    and         al,         0x7f
    out         0x70,       al
    ; -------------------------------
    mov         eax,        [cs:_eax_]
    mov         ebx,        [cs:_ebx_]
    mov         ecx,        [cs:_ecx_]
    mov         edx,        [cs:_edx_]
    mov         esi,        [cs:_esi_]
    mov         edi,        [cs:_edi_]
    mov         ebp,        [cs:_ebp_]
    mov         ds,         [cs:_ds_]
    mov         es,         [cs:_es_]
    mov         fs,         [cs:_fs_]
    mov         gs,         [cs:_gs_]
    mov         sp,         cs
    mov         ss,         sp
    mov         sp,         _flags_
    popf
    mov         ss,         [cs:_ss_]
    mov         esp,        [cs:_esp_]
    call        dword [cs:_cs_ip_]
  
    mov         [cs:_esp_], esp
    mov         [cs:_ss_],  ss
    mov         sp,         cs
    mov         ss,         sp
    mov         sp,         _flags_+2
    pushf 
    mov         [cs:_gs_],  gs
    mov         [cs:_fs_],  fs
    mov         [cs:_es_],  es
    mov         [cs:_ds_],  ds
    mov         [cs:_ebp_], ebp
    mov         [cs:_edi_], edi
    mov         [cs:_esi_], esi
    mov         [cs:_edx_], edx
    mov         [cs:_ecx_], ecx
    mov         [cs:_ebx_], ebx
    mov         [cs:_eax_], eax

    ; -- disable interrupts ----
    cli    
    ; ---- disable nmi ---------
    in          al,         0x70
    or          al,         0x80
    out         0x70,       al
    ; -------------------------
  .reentering_prot_mode:
    lidt        [cs:_p_idtr]
    mov         eax,        cr0
    or          ax,         1
    mov         cr0,        eax
    jmp         0x08:.prot_mode
   
    use32
  .prot_mode:
    lss         esp,        [cs:_o_esp_]
    popfd
    pop         gs
    pop         fs
    pop         es
    pop         ds
    popad
    ret
  
    use16
  
  .int_xx:
    int         0xcc
    retf

  .filler:      db  "ALL WORK AND NO PLAY MAKES JACK A DULL BOY."
                db  "ALL WORK AND NO PLAY MAKES JACK A DULL BOY."
                db  "ALL WORK AND NO PLAY MAKES JACK A DULL BOY."
                db  "ALL WORK AND NO PLAY MAKES JACK A DULL BOY."
    times 512-($-$$) db 0x1
