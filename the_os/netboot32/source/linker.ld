OUTPUT_FORMAT(binary)
ENTRY(Main)
SECTIONS
{
  . = 0x7c00;

  .preamble : 
  {
    _PREAMBLE_BEGIN_ = .;
    *(.preamble)
    _PREAMBLE_END_ = .;
  }

  . = ALIGN(16);
  .text :
  {
    _TEXT_BEGIN_ = .;
    *(.Main)
    *(.text.*)
    *(.text)
    _TEXT_END_ = .;
  }

  . = ALIGN(16);
  .data :
  {
    _ISRTAB_BEGIN_ = .;    
    *(.isrtab)    
    _ISRTAB_END_ = .;

    _INIT_ARRAY_BEGIN_ = .;
    *(.init_array)
    _INIT_ARRAY_END_ = .;

    _DATA_BEGIN_ = .;
    *(.data)
    *(.rodata)
    _DATA_END_ = .;
  }

  .bss :
  {
    _BSS_BEGIN_ = .;
    *(.bss)
    _BSS_END_ = .;
  }

  /DISCARD/ : 
  {    
    *(.eh_frame)
    *(.comment)
    *(.note.GNU-stack)
    *(.group)
  }
}
