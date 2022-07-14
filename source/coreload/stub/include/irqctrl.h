#ifndef __IRQSTUBS_H__
#define __IRQSTUBS_H__

#include "types.h"

#define IRQ_INIT_IRQ0_BIT         0x0001
#define IRQ_INIT_IRQ1_BIT         0x0002
#define IRQ_INIT_IRQ2_BIT         0x0004
#define IRQ_INIT_IRQ3_BIT         0x0008
#define IRQ_INIT_IRQ4_BIT         0x0010
#define IRQ_INIT_IRQ5_BIT         0x0020
#define IRQ_INIT_IRQ6_BIT         0x0040
#define IRQ_INIT_IRQ7_BIT         0x0080
#define IRQ_INIT_IRQ8_BIT         0x0100
#define IRQ_INIT_IRQ9_BIT         0x0200
#define IRQ_INIT_IRQ10_BIT        0x0400
#define IRQ_INIT_IRQ11_BIT        0x0800
#define IRQ_INIT_IRQ12_BIT        0x1000
#define IRQ_INIT_IRQ13_BIT        0x2000
#define IRQ_INIT_IRQ14_BIT        0x4000
#define IRQ_INIT_IRQ15_BIT        0x8000

#define IRQ_ACKNOWLEDGE_BIT       0x0004
#define IRQ_CALL_DEFAULT_BIT      0x0001

#define IRQ_CALL_DEFAULT          IRQ_CALL_DEFAULT_BIT
#define IRQ_DO_NOTHING            0x0000
#define IRQ_ACKNOWLEDGE           IRQ_ACKNOWLEDGE_BIT

#define IRQ_ERROR_BAD_IRQ_NUMBER  -1

void    IRQ_init   (uint16_t mask);
int16_t IRQ_set    (void* callable, uint16_t irq);
void*   IRQ_get    (uint16_t irq);
void    IRQ_mask   (uint16_t mask);
void    IRQ_unmask (uint16_t mask);

#endif
