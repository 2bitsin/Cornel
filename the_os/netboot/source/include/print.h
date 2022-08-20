#ifndef __PRINT_H__
#define __PRINT_H__

#include "types.h"

void init_text();

void print_char(char);
void print_string(const char* value);
void print_string_n(const char* value, size_t n);

void print_hex8 (uint8_t  value);
void print_hex16(uint16_t value);
void print_hex32(uint32_t value);
void print_hex64(uint64_t value);

void print_dec8 (uint8_t  value);
void print_dec16(uint16_t value);
void print_dec32(uint32_t value);
void print_dec64(uint64_t value);


#if (defined(DEBUG) && DEBUG == 1) || (defined(FORCE_DBG_PRINT) && FORCE_DBG_PRINT != 0)
  #define DBG_print_char(c)         print_char(c)
  #define DBG_print_string(s)       print_string(s)
  #define DBG_print_string_n(s, n)  print_string_n(s, n)
  #define DBG_print_hex8(v)         print_hex8(v)
  #define DBG_print_hex16(v)        print_hex16(v)
  #define DBG_print_hex32(v)        print_hex32(v)
  #define DBG_print_hex64(v)        print_hex64(v)
  #define DBG_print_dec8(v)         print_dec8(v)
  #define DBG_print_dec16(v)        print_dec16(v)
  #define DBG_print_dec32(v)        print_dec32(v)
  #define DBG_print_dec64(v)        print_dec64(v)
#else
  #define DBG_print_char(c)         
  #define DBG_print_string(s)
  #define DBG_print_string_n(s, n)
  #define DBG_print_hex8(v)
  #define DBG_print_hex16(v)
  #define DBG_print_hex32(v)
  #define DBG_print_hex64(v)
  #define DBG_print_dec8(v)
  #define DBG_print_dec16(v)
  #define DBG_print_dec32(v)
  #define DBG_print_dec64(v)
#endif

#endif
