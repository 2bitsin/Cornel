#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include "types.hpp"
//#include "assert.hpp"

template <typename T>
inline T _far * const make_farptr(uword seg, uword ofs)
{
  return (T _far * const) ((seg * 0x10000ul) + ofs);
}

template <typename T>
inline void mem_write(uword seg, uword off, T val)
{
  (*make_farptr<T>(seg, off)) = val;
}

template <typename T>
inline T mem_read(uword seg, uword off)
{
  return (*make_farptr<T>(seg, off));
}

inline void memswp(void* lhs, void* rhs, uint16 size)
{
  ubyte* lhs_ptr = (ubyte*)lhs;
  ubyte* rhs_ptr = (ubyte*)rhs;
  ubyte tmp;
  for(uint32 i = 0; i < size; ++i)
  {
    tmp = lhs_ptr[i];
    lhs_ptr[i] = rhs_ptr[i];
    rhs_ptr[i] = tmp;
  }    
}

template <typename T>
inline void swap(T& lhs, T& rhs)
{  
  T tmp = lhs;
  lhs = rhs;
  rhs = tmp;
}

template <typename T, uint32 N>
inline void swap(T (&lhs)[N], T (&rhs)[N])
{
  for(uint32 i = 0; i < N; ++i)
  {
    swap(lhs[i], rhs[i]);
  }
}

inline void memcpy(void* dst, void* src, uint16 size)
{
  ubyte* dst_ptr = (ubyte*)dst;
  ubyte* src_ptr = (ubyte*)src;
  for(uint32 i = 0; i < size; ++i)
    dst_ptr[i] = src_ptr[i];
}

inline void memset(void* dst, ubyte val, uint16 size)
{
  ubyte* dst_ptr = (ubyte*)dst;
  for(uint32 i = 0; i < size; ++i)
    dst_ptr[i] = val;
}

template <typename T>
inline void reverse(T* first, T* last)
{
  --last;
  while(first < last)
  {
    swap(*first, *last);
    --last;
    ++first;
  }
}

#define nullptr ((void*)0)

#endif