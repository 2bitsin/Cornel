#include <cstddef>
#include <cstdint>

#include <cstring>

extern "C"
{
  auto strlen (const char *string) 
    -> std::size_t
  {
    auto currptr = string;
    while (*currptr != '\0')
      ++currptr;
    return currptr - string;    
  }
}