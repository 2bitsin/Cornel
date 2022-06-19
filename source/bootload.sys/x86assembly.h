#ifndef __X86_ASM_H__
#define __X86_ASM_H__

void x86_cli();
#pragma aux x86_cli = "cli"

void x86_sti();
#pragma aux x86_sti = "sti"

void x86_hlt();
#pragma aux x86_hlt = "hlt"

void x86_outw(unsigned short port, unsigned short value);
#pragma aux x86_outb = "out dx, al" parm [dx] [al]

void x86_outb(unsigned short port, unsigned char value);
#pragma aux x86_outw = "out dx, ax" parm [dx] [ax]

unsigned char x86_inb(unsigned short port);
#pragma aux x86_inb = "in al, dx" parm [dx] value [al]

unsigned short x86_inw(unsigned short port);
#pragma aux x86_inw = "in ax, dx" parm [dx] value [ax] 

x86_load_ss_sp(void _far * ss_sp);
#pragma aux x86_load_ss_sp = "mov ss, dx" "mov sp, ax"  parm [dx ax]

#endif
