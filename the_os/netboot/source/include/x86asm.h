#ifndef __X86_ASM_H__
#define __X86_ASM_H__

#include "types.h"

void x86_cli();
#pragma aux x86_cli = "cli"

void x86_sti();
#pragma aux x86_sti = "sti"

void x86_hlt();
#pragma aux x86_hlt = "hlt"

void x86_outw(uint16_t port, uint16_t value);
#pragma aux x86_outb = "out dx, al" parm [dx] [al]

void x86_outb(uint16_t port, uint8_t value);
#pragma aux x86_outw = "out dx, ax" parm [dx] [ax]

uint8_t x86_inb(uint16_t port);
#pragma aux x86_inb = "in al, dx" parm [dx] value [al]

uint16_t x86_inw(uint16_t port);
#pragma aux x86_inw = "in ax, dx" parm [dx] value [ax] 

void x86_load_ss_sp(uint16_t ss, uint16_t sp);
#pragma aux x86_load_ss_sp = "pushf" "cli" "pop bx" "mov ss, dx" "mov sp, ax" "push bx" "popf"  parm [dx] [ax] modify [bx]

void x86_load_sp(uint16_t value);
#pragma aux x86_load_sp = "mov sp, ax" parm [ax];

void x86_load_ss(uint16_t value);
#pragma aux x86_load_ss = "mov ss, ax" parm [ax];

void x86_load_ds(uint16_t value);
#pragma aux x86_load_ds = "mov ds, ax" parm [ax];

void x86_load_es(uint16_t value);
#pragma aux x86_load_es = "mov es, ax" parm [ax];

void x86_load_fs(uint16_t value);
#pragma aux x86_load_fs = "mov fs, ax" parm [ax];

void x86_load_gs(uint16_t value);
#pragma aux x86_load_gs = "mov gs, ax" parm [ax];

uint16_t x86_ds();
#pragma aux x86_ds = "mov ax, ds" value [ax];

uint16_t x86_es();
#pragma aux x86_es = "mov ax, es" value [ax];

uint16_t x86_fs();
#pragma aux x86_fs = "mov ax, fs" value [ax];

uint16_t x86_gs();
#pragma aux x86_gs = "mov ax, gs" value [ax];

uint16_t x86_cs();
#pragma aux x86_cs = "mov ax, cs" value [ax];

void x86_load_all_seg(uint16_t value);
#pragma aux x86_load_all_seg = "mov ds, ax" "mov es, ax" "mov fs, ax" "mov gs, ax" parm [ax];

uint16_t x86_flags16();
#pragma aux x86_flags16 = "pushf" "pop ax" value [ax];

uint32_t x86_flags32();
#pragma aux x86_flags32 = "pushfd" "pop ax" "pop dx" value [dx ax];

void x86_load_flags16(uint16_t value);
#pragma aux x86_load_flags16 = "push ax" "popf" parm [ax];

void x86_load_flags32(uint32_t value);
#pragma aux x86_load_flags32 = "push dx" "push ax" "popfd" parm [dx ax];

void bochs_magic_break();
#pragma aux bochs_magic_break = "xchg bx, bx"

byte read_bda_byte(uint8_t offset);
#pragma aux read_bda_byte = "xor bh, bh" "mov al, gs:[0x400 + bx]" parm [bl] value [al]
word read_bda_word(uint8_t offset);
#pragma aux read_bda_word = "xor bh, bh" "mov ax, gs:[0x400 + bx]" parm [bl] value [ax]
dword read_bda_dword(uint8_t offset);
#pragma aux read_bda_dword = "xor bh, bh" "mov ax, gs:[0x400 + bx]" "mov dx, gs:[0x400 + bx + 2]" parm [bl] value [dx ax]

void _far* x86_es_bx();
#pragma aux x86_es_bx = "mov ax, es" value [ax bx]

void _far* x86_ss_sp();
#pragma aux x86_ss_sp = "mov ax, ss" "mov bx, sp" value [ax bx]

#endif
