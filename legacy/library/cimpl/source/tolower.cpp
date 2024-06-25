#include <cstddef>
#include <cstdint>
#include <cctype>

extern "C"
int tolower(int value)
{
  if (value >= 'A' && value <= 'Z')
    return value - 'A' + 'a';
  return value;
}

