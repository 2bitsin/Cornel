#include <cstddef>
#include <cstdint>
#include <cctype>

extern "C"
int toupper(int value)
{
  return __builtin_toupper(value);
}

