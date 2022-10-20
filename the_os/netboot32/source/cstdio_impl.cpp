#include <cstdio>
#include <cerrno>
#include <hardware/console.hpp>

static int G_errno { 0 };

namespace std
{  
  extern "C"
  {
    int* __errno_location() 
    {
      return &G_errno;
    }

    auto stdin  = (FILE *)0;    
    auto stdout = (FILE *)1;    
    auto stderr = (FILE *)2;    

    int putchar(int char_v)
    {      
      return fputc (char_v, stdout);
    }

    int puts(const char *s)
    {
      return fputs (s, stdout);
    }

    int putc(int char_v, FILE* file_v)
    {
      return fputc (char_v, file_v);
    }

    int fputc(int char_v, FILE* file_v)
    {
      errno = 0;
      if (file_v == stdout || file_v == stderr)
      {
        console::instance().write_char (char_v);
        return char_v;
      }
      errno = EBADF;
      return EOF;
    }

    int fputs(const char* str_v, FILE* file_v)
    {
      while (*str_v) 
      {
        if (EOF == fputc (*str_v++, file_v))
          return EOF;
      }
      return 1;
    }

    unsigned int fwrite(const void* ptr_v, size_t size_v, size_t count_v, FILE* file_v)
    {
      auto char_p = (const char*)ptr_v;

      for (size_t j = 0u; j < count_v; ++j)
      for (size_t i = 0u; i < size_v; ++i)
      {
        if (EOF == fputc (*char_p, file_v))
          return j;
        char_p++;
      }
      return count_v;
    }
  }
}