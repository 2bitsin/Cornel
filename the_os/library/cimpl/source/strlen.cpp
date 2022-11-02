#include <cstddef>
#include <cstdint>
#include <cstring>

extern "C"
auto strlen (const char *string) 
  -> std::size_t
{   
  auto len = 0u;
  while (*string++)
    len++;
  return len;
}
