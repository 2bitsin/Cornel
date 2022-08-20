#ifndef __TYPES_H__
#define __TYPES_H__

typedef unsigned char     uint8_t, ubyte, byte;
typedef unsigned short    uint16_t, uword, word, uintptr_t, size_t;
typedef unsigned long     uint32_t, udword, dword, uintfarptr_t, farsize_t;
typedef unsigned __int64  uint64_t, uqword, qword;

typedef signed char       int8_t, ibyte;
typedef signed int        int16_t, iword, intptr_t, ptrdiff_t, ssize_t;
typedef signed long       int32_t, idword, intfarptr_t, sfarsize_t;
typedef signed __int64    int64_t, iqword;

typedef void*             nullptr_t;
typedef void _far*        nullfarptr_t;

#define nullptr           ((nullptr_t)0)
#define nullfarptr        ((nullfarptr_t)0)

#endif

