#include <cstddef>
#include <cstdint>
#include <cstring>

extern "C"
auto strlen (const char *string) 
  -> std::size_t
{   
  return __builtin_strlen(string);
}
