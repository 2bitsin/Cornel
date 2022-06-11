#ifndef __UTILS_HPP__
#define __UTILS_HPP__

#include "types.hpp"

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


#endif