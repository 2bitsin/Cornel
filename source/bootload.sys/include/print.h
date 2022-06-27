#ifndef __PRINT_H__
#define __PRINT_H__

void print_char(char);
void print_string(const char* value);
void print_string_n(const char* value, unsigned n);

void print_hex8(unsigned char value);
void print_hex16(unsigned short value);
void print_hex32(unsigned long value);
void print_hex64(unsigned __int64 value);

void print_dec8(unsigned char value);
void print_dec16(unsigned short value);
void print_dec32(unsigned long value);
void print_dec64(unsigned __int64 value);

#endif
