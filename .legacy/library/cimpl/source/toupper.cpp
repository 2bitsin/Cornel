#include <cstddef>
#include <cstdint>
#include <cctype>

extern "C"
int toupper(int value)
{
  if (value >= 'a' && value <= 'z')
    return value - 'a' + 'A';
  return value;
}

