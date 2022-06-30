#ifndef __LONGDIV_H__
#define __LONGDIV_H__

#pragma pack(push, 1)

typedef struct u_longdiv_t
{
  union
  {
    struct
    {
      unsigned long q;
      unsigned long r;
    };
    unsigned __int64 d;  
  };
} u_longdiv_type;

typedef struct i_longdiv_t
{
  union
  {
    struct
    {
      long q; // + 0
      long r; // + 4
    };
    __int64 d;
  };
} i_longdiv_type;

#pragma pack(pop)

void long_64_udiv_32 (u_longdiv_type* divident_or_result, const unsigned long* divider);
void long_64_idiv_32 (i_longdiv_type* divident_or_result, const long* divider);

#endif

