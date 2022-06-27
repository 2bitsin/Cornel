#ifndef __MEMMAP_H__
#define __MEMMAP_H__

int MM_populate();

typedef struct MM_entry_t
{
  unsigned __int64 base;
  unsigned __int64 size;
  unsigned long type;
} MM_entry_type;


#endif

