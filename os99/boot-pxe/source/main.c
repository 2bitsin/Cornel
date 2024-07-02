#include <stdio.h>
#include <stdint.h>
#include <stddef.h>

typedef void far* pfvoid_t;
typedef void const far* pfcvoid_t;

typedef struct 
{
  uint16_t di, si, bp, sp, 
    bx, dx, cx, ax, gs, fs, 
    es, ds, intnum, ip, cs, 
    flags;
} int_regs_t;


inline pfvoid_t get_ivt_entry(uint8_t v_index) 
{
  return ((pfvoid_t far *)0x0u)[v_index];
}

inline void set_ivt_entry(uint8_t v_index, pfvoid_t v_value) 
{
  ((pfvoid_t far *)0x0u)[v_index] = v_value;
}

void __far dos_handle_44(int_regs_t far* r) {
  __asm
  { 
    xchg bx, bx 
    mov ax, 0x1234 
  };  
}

void init_dos_calls()
{
  pfvoid_t far * v_table = (pfvoid_t far *)get_ivt_entry(0xff);
  v_table[0x44] = &dos_handle_44;
}


int main(int argc, char** argv) 
{     
  __asm{ xchg bx, bx };
  init_dos_calls();
  puts("Hello World!\n");
  return 0;
}

