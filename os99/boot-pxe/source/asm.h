#pragma once


void __debugbreak();
#pragma aux __debugbreak = "xchg bx, bx";
