.386
.model tiny

_TEXT segment use16 para public 'CODE'

_PMODE_disable_nmi proc near
  push      ax
  in        al,             0x70
  or        al,             0x80
  out       0x70,           al  
  pop       ax
  ret
_PMODE_disable_nmi endp

_PMODE_enable_nmi proc near
  push      ax
  in        al,             0x70
  and       al,             0x7f
  out       0x70,           al  
  pop       ax
  ret
_PMODE_enable_nmi endp

_PMODE_init_flat_real_mode proc near public
  
  pushad 
  push  ds 
  push  es
  push  fs
  push  gs

  cli
  call  _PMODE_disable_nmi

  mov   [save_sp],    sp  
  mov   ax,           ss
  mov   [save_ss],    ax

  mov   ax,           ss
  movzx eax,          ax
  shl   eax,          4
  movzx esp,          sp
  add   eax,          esp
  mov   [save_esp],   esp
  db    0x0f, 0x01, 0x16  ; lgdt [GDTR_temp]
  dw    offset GDTR_temp
  db    0x0f, 0x20, 0xc0  ; mov eax, cr0
  xor   ax,           1
  db    0x0f, 0x22, 0xc0  ; mov cr0, eax
  db    0xea              ; jmp 0x8:_PMODE_init32
  dw    offset _PMODE_init32 + G_LOAD_ADDRESS - G_STACK_SIZE
  dw    0x0008
 
_PMODE_backto16:
  db    0x0f, 0x20, 0xc0  ; mov eax, cr0
  xor   ax,           1
  db    0x0f, 0x22, 0xc0  ; mov cr0, eax
  db    0xea              ; jmp 0x18:_PMODE_backtoRm
  dw    offset _PMODE_backtorm + G_LOAD_ADDRESS - G_STACK_SIZE
  dw    0x0000

_PMODE_backtorm:
  db    0xea              ; jmp LOAD_SEGMENT:_PMODE_fix_cs
  dw    offset _PMODE_fix_cs
  dw    (G_LOAD_ADDRESS - G_STACK_SIZE)/16

_PMODE_fix_cs:

  call  _PMODE_enable_nmi
  sti
  pop   gs 
  pop   fs
  pop   es
  pop   ds
  popad
  ret

_PMODE_init_flat_real_mode endp

_TEXT ends

_TEXT32 segment use32 para public 'CODE'

_PMODE_init32 proc near

  mov   ax,           0x10
  mov   ds,           ax
  mov   es,           ax
  mov   fs,           ax
  mov   gs,           ax

  db    0xea                    ; jmp 0x18:_PMODE_backto16
  dd    offset _PMODE_backto16
  dw    0x0018

_PMODE_init32 endp
_TEXT32 ends 

_DATA segment use16 para public 'DATA'

  public GDT_temp:proc 
  GDT_temp:
    dd 0x00000000, 0x00000000 ; 0x0000 = null
    dd 0x0000ffff, 0x00cf9e00 ; 0x0008 = 32bit code 
    dd 0x0000ffff, 0x00cf9200 ; 0x0010 = 32bit data/stack
    dw 0xffff, offset G_LOAD_ADDRESS - G_STACK_SIZE, 0x9e00, 0x0000 ; 0x0018 = 16bit code base = 0x6000
    dd 0x0000ffff, 0x00009200 ; 0x0020 = 16bit data/stack
    dd 0x0000ffff, 0x00009e00 ; 0x0028 = 16bit code base = 0


  public GDTR_temp:proc 
  GDTR_temp:
    dw offset GDTR_temp - offset GDT_temp
    dd offset GDT_temp + G_LOAD_ADDRESS - G_STACK_SIZE

_DATA ends

_BSS segment use16 public 'BSS'

  save_ss dw ?
  save_sp dw ?

  save_esp dd ? 

_BSS ends

end
