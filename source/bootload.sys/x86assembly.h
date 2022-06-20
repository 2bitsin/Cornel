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

void x86_load_ss_sp(void _far * ss_sp);
#pragma aux x86_load_ss_sp = "mov ss, dx" "mov sp, ax"  parm [dx ax]

void x86_load_ds(unsigned short value);
#pragma aux x86_load_ds = "mov ds, ax" parm [ax];

void x86_load_es(unsigned short value);
#pragma aux x86_load_es = "mov es, ax" parm [ax];

void x86_load_fs(unsigned short value);
#pragma aux x86_load_fs = "mov fs, ax" parm [ax];

void x86_load_gs(unsigned short value);
#pragma aux x86_load_gs = "mov gs, ax" parm [ax];

unsigned short x86_ds();
#pragma aux x86_ds = "mov ax, ds" value [ax];

unsigned short x86_es();
#pragma aux x86_es = "mov ax, es" value [ax];

unsigned short x86_fs();
#pragma aux x86_fs = "mov ax, fs" value [ax];

unsigned short x86_gs();
#pragma aux x86_gs = "mov ax, gs" value [ax];

unsigned short x86_cs();
#pragma aux x86_cs = "mov ax, cs" value [ax];


#endif
