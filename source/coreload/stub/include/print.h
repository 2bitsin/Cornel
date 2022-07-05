#ifndef __PRINT_H__
#define __PRINT_H__

#include "types.h"

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

#endif
