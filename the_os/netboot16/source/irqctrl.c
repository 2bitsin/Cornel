#include "irqctrl.h"
#include "x86asm.h"
#include "memory.h"
#include "print.h"
#include "bioskey.h"

#pragma pack(push, 1)
typedef struct farptr_t
{
  void* off;
  uint16_t seg;  
} farptr_type;
#pragma pack(pop)

static uint16_t __cdecl IRQ_noop (uint16_t irq_n)
{
  return IRQ_CALL_DEFAULT;
}

void __cdecl PIT_irq (int irq_n);
void __cdecl SER_irq (int irq_n);

void __cdecl IRQ0_stub (int irq_n);
void __cdecl IRQ1_stub (int irq_n);
void __cdecl IRQ2_stub (int irq_n);
void __cdecl IRQ3_stub (int irq_n);
void __cdecl IRQ4_stub (int irq_n);
void __cdecl IRQ5_stub (int irq_n);
void __cdecl IRQ6_stub (int irq_n);
void __cdecl IRQ7_stub (int irq_n);
void __cdecl IRQ8_stub (int irq_n);
void __cdecl IRQ9_stub (int irq_n);
void __cdecl IRQ10_stub (int irq_n);
void __cdecl IRQ11_stub (int irq_n);
void __cdecl IRQ12_stub (int irq_n);
void __cdecl IRQ13_stub (int irq_n);
void __cdecl IRQ14_stub (int irq_n);
void __cdecl IRQ15_stub (int irq_n);

uint8_t const IRQ_map [16] = 
{
  0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,   // Master PIC
  0x70, 0x71, 0x72, 0x73, 0x74, 0x75, 0x76, 0x77    // Slave PIC
};

void* volatile IRQ_handler_table [16] =
{
  /* 0  */ &PIT_irq,
  /* 1  */ &IRQ_noop,
  /* 2  */ &IRQ_noop,
  /* 3  */ &SER_irq,
  /* 4  */ &SER_irq,
  /* 5  */ &IRQ_noop,
  /* 6  */ &IRQ_noop,
  /* 7  */ &IRQ_noop,
  /* 8  */ &IRQ_noop,
  /* 9  */ &IRQ_noop,
  /* 10 */ &IRQ_noop,
  /* 11 */ &IRQ_noop,
  /* 12 */ &IRQ_noop,
  /* 13 */ &IRQ_noop,
  /* 14 */ &IRQ_noop,
  /* 15 */ &IRQ_noop
};

farptr_type volatile IRQ_saved_isr_table[16] = 
{ 
 
  /* 0  */ {&IRQ0_stub,   G_BASE_ADDRESS/16},
  /* 1  */ {&IRQ1_stub,   G_BASE_ADDRESS/16},
  /* 2  */ {&IRQ2_stub,   G_BASE_ADDRESS/16},
  /* 3  */ {&IRQ3_stub,   G_BASE_ADDRESS/16},
  /* 4  */ {&IRQ4_stub,   G_BASE_ADDRESS/16},
  /* 5  */ {&IRQ5_stub,   G_BASE_ADDRESS/16},
  /* 6  */ {&IRQ6_stub,   G_BASE_ADDRESS/16},
  /* 7  */ {&IRQ7_stub,   G_BASE_ADDRESS/16},
  /* 8  */ {&IRQ8_stub,   G_BASE_ADDRESS/16},
  /* 9  */ {&IRQ9_stub,   G_BASE_ADDRESS/16},
  /* 10 */ {&IRQ10_stub,  G_BASE_ADDRESS/16},
  /* 11 */ {&IRQ11_stub,  G_BASE_ADDRESS/16},
  /* 12 */ {&IRQ12_stub,  G_BASE_ADDRESS/16},
  /* 13 */ {&IRQ13_stub,  G_BASE_ADDRESS/16},
  /* 14 */ {&IRQ14_stub,  G_BASE_ADDRESS/16},
  /* 15 */ {&IRQ15_stub,  G_BASE_ADDRESS/16}
 };

static uint16_t IRQ_init_mask = 0;
volatile uint16_t IRQ_save_ss = 'SS';
volatile uint16_t IRQ_save_sp = 'PS';

volatile uint32_t IRQ_stack_bits [256] = 
{
  #define IRQ_STACK_FILLER5 'SQRI', 'KCAT'
  #define IRQ_STACK_FILLER4 IRQ_STACK_FILLER5, IRQ_STACK_FILLER5
  #define IRQ_STACK_FILLER3 IRQ_STACK_FILLER4, IRQ_STACK_FILLER4, IRQ_STACK_FILLER4, IRQ_STACK_FILLER4
  #define IRQ_STACK_FILLER2 IRQ_STACK_FILLER3, IRQ_STACK_FILLER3, IRQ_STACK_FILLER3, IRQ_STACK_FILLER3
  #define IRQ_STACK_FILLER1 IRQ_STACK_FILLER2, IRQ_STACK_FILLER2, IRQ_STACK_FILLER2, IRQ_STACK_FILLER2

  IRQ_STACK_FILLER1

  #undef IRQ_STACK_FILLER1
  #undef IRQ_STACK_FILLER2
  #undef IRQ_STACK_FILLER3
  #undef IRQ_STACK_FILLER4
  #undef IRQ_STACK_FILLER5    
};
uint32_t IRQ_stack_top[2] = {'CATS', 'POTK'};

int16_t IRQ_set(void* callable, uint16_t irq)
{
  uint16_t flags;
  if (irq > 15) 
    return IRQ_ERROR_BAD_IRQ_NUMBER;
  flags = x86_flags16();
  x86_cli();
  IRQ_handler_table[irq] = callable;
  x86_load_flags16(flags);
  return 0;
}

void* IRQ_get(uint16_t irq)
{
  uint16_t flags;
  void* value;
  if (irq > 15)
    return nullptr;
  flags = x86_flags16();
  x86_cli();
  value = IRQ_handler_table[irq];
  x86_load_flags16(flags);
  return value;  
}

void IRQ_init(uint16_t mask)
{  
  uint16_t i, index, flags; 
  farptr_type tmp;

  volatile farptr_type _far* G_isr_table;
  flags = x86_flags16();
  x86_cli();
  G_isr_table = (volatile farptr_type _far*)0;
  mask &= ~IRQ_init_mask;
  IRQ_init_mask |= mask;
  for (i = 0; i < 16; ++i, mask >>= 1) 
  {
    if (!(mask & 1))
      continue;
    index = IRQ_map[i];
    tmp.seg = G_isr_table[index].seg;
    tmp.off = G_isr_table[index].off;
    G_isr_table[index].seg = IRQ_saved_isr_table[i].seg;
    G_isr_table[index].off = IRQ_saved_isr_table[i].off;
    IRQ_saved_isr_table[i].seg = tmp.seg;
    IRQ_saved_isr_table[i].off = tmp.off;
  }
  x86_load_flags16(flags);
}

void IRQ_mask(uint16_t m) 
{
  uint8_t lower = m & 0xff;
  uint8_t upper = m >> 8;
  if (lower != 0) x86_outb(0x21, x86_inb(0x21) | lower);
  if (upper != 0) x86_outb(0xa1, x86_inb(0xa1) | upper);
}
 
void IRQ_unmask(uint16_t m) 
{
  uint8_t lower = m & 0xff;
  uint8_t upper = m >> 8;
  if (lower != 0) x86_outb(0x21, x86_inb(0x21) & ~lower);
  if (upper != 0) x86_outb(0xa1, x86_inb(0xa1) & ~upper);
}
