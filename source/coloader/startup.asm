.model tiny
.386

  DGROUP  GROUP  _DATA,_BSS,_STACK

_TEXT SEGMENT 'CODE' PARA PUBLIC USE16
  ASSUME  CS:DGROUP,DS:DGROUP,ES:DGROUP

  extern _coloader_main:proc

_coloader_init proc near public
  db          0xea
  dw          offset _coloader_init_l0
  dw          0x0000
_coloader_init_l0:
  mov         ax,       cs
  mov         ds,       ax
  mov         ss,       ax
  mov         es,       ax
  xor         ax,       ax
  mov         sp,       offset _coloader_init 
  call        _coloader_main
  int         0x19
  push        0xffff
  push        0x0000
  retf
_coloader_init endp

_TEXT ENDS

_BSS SEGMENT 'BSS' PARA PUBLIC USE16
_BSS ENDS

_STACK SEGMENT 'STACK' PARA PUBLIC USE16
_STACK ENDS

_DATA SEGMENT 'DATA' PARA PUBLIC USE16
    
_small_code_:
  public _small_code_

__compiled_under_generic:
  public __compiled_under_generic

_DATA ENDS

END