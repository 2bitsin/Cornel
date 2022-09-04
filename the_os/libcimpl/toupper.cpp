#include <cctype>
#include <cstddef>
#include <cstdint>

extern "C"
int toupper(int value)
{
  if (value >= 'a' && value <= 'z') 
  {
    return (value - 'a') + 'A';
  }
  return value;
}
