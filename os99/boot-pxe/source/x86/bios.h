#pragma once

#include <stddef.h>
#include <stdint.h>

#include "core/sysinfo.h"

uint16_t __watcall BIOS_query_memmap(memmap_item far* v_buff, uint32_t *v_size, uint32_t *v_next);

uint16_t __watcall BIOS_serial_addr (uint8_t port_number);
uint16_t __watcall BIOS_serial_init (uint8_t port_number, uint8_t config_byte = 0xe3); // baud=96000, parity=none, stop=1, data=8 
uint16_t __watcall BIOS_serial_stat (uint8_t port_number);
uint16_t __watcall BIOS_serial_puts (uint8_t port_number, char const far* string);
uint16_t __watcall BIOS_serial_getc (uint8_t port_number);
uint16_t __watcall BIOS_serial_putc (uint8_t port_number, uint8_t value);

#pragma aux BIOS_serial_init = "mov ax, 0x0000" "int 0x14" parm [dx] [al] value [ax] modify [ax];
#pragma aux BIOS_serial_putc = "mov ah, 0x0001" "int 0x14" parm [dx] [al] value [ax] modify [ax];
#pragma aux BIOS_serial_getc = "mov ah, 0x0002" "int 0x14" parm [dx] value [ax] modify [ax];
#pragma aux BIOS_serial_stat = "mov ax, 0x0300" "int 0x14" parm [dx] value [ax] modify [ax];

