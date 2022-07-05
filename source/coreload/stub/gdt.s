.386
.model tiny

_DATA segment use16 para public 'DATA'

  _GDT_bits proc near public
    dd 0x00000000, 0x00000000 ; 0x0000 = null
    dd 0x0000ffff, 0x00cf9e00 ; 0x0008 = 32bit code 
    dd 0x0000ffff, 0x00cf9200 ; 0x0010 = 32bit data/stack
    dd 0x0000ffff, 0x00009e00 ; 0x0018 = 16bit code
    dd 0x0000ffff, 0x00009200 ; 0x0020 = 16bit data/stack
  _GDT_bits endp
    
  _GDTR_bits proc near public
    dw offset _GDTR_bits - offset _GDT_bits
    dd offset _GDT_bits + G_BASE_ADDRESS
  _GDTR_bits endp

_DATA ends

end

