#ifndef __CHARCONV_HPP__
#define __CHARCONV_HPP__

#include "types.hpp"

char* to_chars(char* first, char* last, uint32 value, int base = 10);
char* to_chars(char* first, char* last, sint32 value, int base = 10);
char* to_chars(char* first, char* last, uint16 value, int base = 10);
char* to_chars(char* first, char* last, sint16 value, int base = 10);
char* to_chars(char* first, char* last, uint8 value, int base = 10);
char* to_chars(char* first, char* last, sint8 value, int base = 10);
char* to_chars(char* first, char* last, int value, int base = 10);
char* to_chars(char* first, char* last, unsigned int value, int base = 10);

char* to_hex(char* first, char* last, uint64 value);
char* to_hex(char* first, char* last, uint32 value);
char* to_hex(char* first, char* last, uint16 value);
char* to_hex(char* first, char* last, uint8 value);

char* to_hex(char* first, char* last, sint64 value);
char* to_hex(char* first, char* last, sint32 value);
char* to_hex(char* first, char* last, sint16 value);
char* to_hex(char* first, char* last, sint8 value);

char* to_hex(char* first, char* last, int value);
char* to_hex(char* first, char* last, unsigned int value);



#endif