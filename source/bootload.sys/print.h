#ifndef __PRINT_H__
#define __PRINT_H__

void print_char(const char);
void print_string(const char _far* value);
void print_hex8(const unsigned char value);
void print_hex16(const unsigned short value);
void print_hex32(const unsigned long value);
void print_hex64(const unsigned __int64 value);

#endif
