#include <hardware/x86/assembly.hpp>
#include <memory/block_list.hpp>
#include <netboot/panick.hpp>
#include <textio/logger.hpp>
#include <utils/macros.hpp>

#include <cstdlib>
#include <cstdarg>

declare_module(Runtime);

CO_PUBLIC 
[[noreturn]]
void abort()
{
  using namespace textio::fmt;
  Glog.fatal<"System halted, press Ctrl+Alt+Del to reboot.">();
  for(;;) x86arch::yield(); 
}

namespace __gnu_cxx
{
  int __snprintf_lite(char *__buf, size_t __bufsize, const char *__fmt, va_list __ap);
}

namespace std
{
  [[noreturn]] 
  void __throw_bad_alloc()
  {
    using namespace textio::fmt;
    Glog.fatal<"Bad allocation exception.">();
    std::abort();
  }

  [[noreturn]]
  void __throw_bad_cast()
  {
    using namespace textio::fmt;
    Glog.fatal<"Bad cast exception.">();
    std::abort();
  }  

  [[noreturn]] 
  void __throw_length_error(char const* err)
  {
    using namespace textio::fmt;
    Glog.fatal<"Length error ({:s}).">(err);
    std::abort();
  }


  [[noreturn]] 
  void __throw_invalid_argument(char const* err)
  {    
    using namespace textio::fmt;
    Glog.fatal<"Invalid argument ({:s}).">(err);
    std::abort();
  }

  [[noreturn]] 
  void __throw_runtime_error(char const* err) 
  {
    using namespace textio::fmt;
    Glog.fatal<"Runtime {:s}.">(err);
    std::abort();
  }

  [[noreturn]]
  void __throw_logic_error(char const* err)
  {
    using namespace textio::fmt;
    Glog.fatal<"Logic error ({:s}).">(err);
    std::abort();
  }

  [[noreturn]]
  void __throw_bad_array_new_length()
  {
    using namespace textio::fmt;
    Glog.fatal<"Bad array new length.">();
    std::abort();
  }

  [[noreturn]]
  void __throw_out_of_range_fmt(char const* __fmt, ...)
  {
    using namespace textio::fmt;    

    const size_t __len = __builtin_strlen(__fmt);
    // We expect at most 2 numbers, and 1 short string. The additional
    // 512 bytes should provide more than enough space for expansion.
    const size_t __alloca_size = __len + 512;
    char *const __s = static_cast<char*>(__builtin_alloca(__alloca_size));
    va_list __ap;

    va_start(__ap, __fmt);
    __gnu_cxx::__snprintf_lite(__s, __alloca_size, __fmt, __ap);
    
    Glog.fatal<"Out of range : {:s}.">(__s);

    va_end(__ap);  // Not reached.

    std::abort();
  }

}

