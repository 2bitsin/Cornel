#ifndef __TYPES_HPP__
#define __TYPES_HPP__


typedef unsigned char     uint8,  ubyte;
typedef unsigned short    uint16, uword;
typedef unsigned long     uint32, udword;
typedef unsigned __int64  uint64, uqword;

typedef signed char     sint8,  sbyte;
typedef signed short    sint16, sword;
typedef signed long     sint32, sdword;
typedef signed __int64  sint64, sqword;

#define static_assert(X, Y) typedef char __static_assert[(X) ? 1 : -1];

static_assert(sizeof(uint8 ) == 1, "uint8 is not 1 byte");
static_assert(sizeof(uint16) == 2, "uint16 is not 2 bytes");
static_assert(sizeof(uint32) == 4, "uint32 is not 4 bytes");
static_assert(sizeof(uint64) == 8, "uint64 is not 8 bytes");
static_assert(sizeof(sint8 ) == 1, "sint8 is not 1 byte");
static_assert(sizeof(sint16) == 2, "sint16 is not 2 bytes");
static_assert(sizeof(sint32) == 4, "sint32 is not 4 bytes");
static_assert(sizeof(sint64) == 8, "sint64 is not 8 bytes");

#undef static_assert

#endif