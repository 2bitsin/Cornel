FORMAT RAW BIN
OPTION
  DOSSEG,
  VERBOSE,
  NODEFAULTLIBS,
  START=_entry,
  STACK=0,
  OFFSET=0x1c00
ORDER
  CLNAME STACK
    SEGMENT _STACK
  CLNAME PROLOGUE
    SEGMENT _PROLOGUE
  CLNAME DATA
    SEGMENT CONST
    SEGMENT CONST2
    SEGMENT _DATA
  CLNAME CODE
    SEGMENT _TEXT
  CLNAME CODE32
    SEGMENT _TEXT32
  CLNAME BSS
    SEGMENT _BSS
  CLNAME EPILOGUE
    SEGMENT _EPILOGUE


