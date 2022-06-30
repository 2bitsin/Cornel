#include "error.h"
#include "print.h"

void print_error_15h(int error)
{
  print_string("Error : ");
  print_string(error_string_15h(error));
  print_string(" (");
  print_hex16(error);
  print_string(")");
}

const char* error_string_15h(int error)
{
  switch(error)
  {
  case 0x80: 
    return "Invalid command";
  case 0x86: 
    return "Function not supported";
  default: 
    return "Unknown error";
  }
}
