#pragma once


void __debugbreak();

#pragma aux __debugbreak = "xchg bx, bx" _parm [bx] _value [bx];

void __debugbreak_num(uint16_t);

#pragma aux __debugbreak_num = "xchg bx, bx" _parm [bx] _value [bx];
