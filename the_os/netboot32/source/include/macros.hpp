#pragma once

#define CO_CDECL      __attribute__((cdecl))
#define CO_SECTION(X) __attribute__((section(X)))
#define CO_PUBLIC     extern "C" CO_CDECL