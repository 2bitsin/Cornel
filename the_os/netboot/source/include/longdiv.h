#ifndef __LONGDIV_H__
#define __LONGDIV_H__

#include "types.h"

#pragma pack(push, 1)

typedef struct u_longdiv_t
{
  union
  {
    struct
    {
      uint32_t q;
      uint32_t r;
    };
    uint64_t d;  
  };
} u_longdiv_type;

typedef struct i_longdiv_t
{
  union
  {
    struct
    {
      int32_t q; // + 0
      int32_t r; // + 4
    };
    int64_t d;
  };
} i_longdiv_type;

#pragma pack(pop)

void long_64_udiv_32 (u_longdiv_type* divident_or_result, const uint32_t* divider);
void long_64_idiv_32 (i_longdiv_type* divident_or_result, const int32_t* divider);

#endif

