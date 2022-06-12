#include "charconv.hpp"
#include "utility.hpp"
#include "assert.hpp"

inline char digits(unsigned x)
{
  return x > 9 ? x + 'A' - 10 : x + '0';
}

char* to_chars(char* first, char* last, uint32 value, int base)
{  
  assert(base > 1 && base <= 36);
  
  assert(first != nullptr && last != nullptr && first < last);

  char* current = first;
  do
  {
    *current = digits(value % base);    
    value /= base;
    ++current;
  }
  while(current != last && value != 0);
 
  reverse(first, current);
  return current;
};

char* to_chars(char* first, char* last, sint32 value, int base)
{
  if(value < 0)
  {
    *first = '-';
    ++first;
    value = -value;
  }
  return to_chars(first, last, (uint32)value, base);
}


char* to_chars(char* first, char* last, uint16 value, int base)
{
  return to_chars(first, last, (uint32)value, base);
}

char* to_chars(char* first, char* last, sint16 value, int base)
{
  return to_chars(first, last, (sint32)value, base);
}

char* to_chars(char* first, char* last, uint8 value, int base)
{
  return to_chars(first, last, (uint32)value, base);
}

char* to_chars(char* first, char* last, sint8 value, int base)
{
  return to_chars(first, last, (sint32)value, base);
}

char* to_chars(char* first, char* last, int value, int base)
{
  return to_chars(first, last, (sint32)value, base);
}

char* to_chars(char* first, char* last, unsigned int value, int base)
{
  return to_chars(first, last, (uint32)value, base);
}

char *to_hex(char *first, char *last, sint64 value)
{
  return to_hex(first, last, (uint64)value);
}

char *to_hex(char *first, char *last, sint32 value)
{
  return to_hex(first, last, (uint32)value);
}

char *to_hex(char *first, char *last, sint16 value)
{
  return to_hex(first, last, (uint16)value);
}

char *to_hex(char *first, char *last, sint8 value)
{
  return to_hex(first, last, (uint8)value);
}

char *to_hex(char *first, char *last, uint64 value)
{  
  assert(first != nullptr && last != nullptr && last - first >= 16);
  uint32 const* p = (uint32 const*)&value;
  to_hex(first, first+8, p[1]);
  to_hex(first+8, last, p[0]);
  return last+16;
}

char *to_hex(char *first, char *last, uint32 value)
{  
  assert(first != nullptr && last != nullptr && last - first >= 8);
  for(int i = 0; i < 8; ++i)
    first[i] = digits(value >> ((8 - i) * 4));
  return first+8;
}

char *to_hex(char *first, char *last, uint16 value)
{  
  assert(first != nullptr && last != nullptr && last - first >= 4);
  for(int i = 0; i < 4; ++i)
    first[i] = digits(value >> ((4 - i) * 4));
  return first+4;
}

char *to_hex(char *first, char *last, uint8 value)
{  
  assert(first != nullptr && last != nullptr && last - first >= 2);
  for(int i = 0; i < 2; ++i)
    first[i] = digits(value >> ((2 - i) * 4));
  return first+2;
}
char* to_hex(char* first, char* last, int value)
{
  return to_hex(first, last, (sint32)value);
}

char* to_hex(char* first, char* last, unsigned int value);
{
  return to_hex(first, last, (uint32)value);
}
