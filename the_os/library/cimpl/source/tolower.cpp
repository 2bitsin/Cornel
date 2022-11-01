#include <cstddef>
#include <cstdint>
#include <cctype>

extern "C"
int tolower(int value)
{
  return __builtin_tolower(value);
}

