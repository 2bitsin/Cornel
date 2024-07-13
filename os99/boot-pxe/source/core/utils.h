#pragma once


uint16_t byte_swap_u16(uint16_t);

#pragma aux byte_swap_u16 = "xchg al, ah" parm [ax] value [ax];

