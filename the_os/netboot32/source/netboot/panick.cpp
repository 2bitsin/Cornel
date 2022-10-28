#include <netboot/panick.hpp>
#include <memory/block_list.hpp>
#include <hardware/x86assembly.hpp>
#include <textio/format.hpp>
#include <utils/macros.hpp>

#include <cstdlib>

CO_PUBLIC 
[[noreturn]]
void abort()
{
  using namespace textio::fmt;
  format_to<"Halting system, press Ctrl+Alt+Del to reboot.\n">(stdout);
  for(;;) x86arch::yield(); 
}

[[noreturn]]
void panick::cant_enable_atwenty() noexcept
{
  using namespace textio::fmt;
  format_to<"#{:03d} - Unable to enable address line 20\n">(stdout, __COUNTER__);
  std::abort();
}

[[noreturn]] 
void panick::out_of_memory(std::size_t size) noexcept
{
  using namespace textio::fmt;
  format_to<"#{:03d} - Out of memory, unable to allocate {:d} bytes\n">(stdout, __COUNTER__, size);
  std::abort();
}

[[noreturn]] 
void panick::invalid_free(void* ptr) noexcept
{
  using namespace textio::fmt;
  format_to<"#{:03d} - Heap deallocation failed ({:08p}), possible corruption.\n">(stdout, __COUNTER__, ptr);
  std::abort();
}

[[noreturn]] 
void panick::out_of_memory(std::size_t size, struct ::block_list const& blist) noexcept
{
  using namespace textio::fmt;
  format_to<"#{:03d} - Out of base memory, unable to allocate {:d} bytes\n">(stdout, __COUNTER__, size);
  pretty_print(blist, stdout);
  std::abort();
}

[[noreturn]] 
void panick::invalid_free(void* ptr, struct ::block_list const& blist) noexcept
{
  using namespace textio::fmt;
  format_to<"#{:03d} - Base heap deallocation failed ({:08p}), possible corruption.\n">(stdout, __COUNTER__, ptr);
  pretty_print(blist, stdout);
  std::abort();
}

[[noreturn]] 
void panick::out_of_memory_ext(std::size_t size, struct ::block_list const& blist) noexcept
{
  using namespace textio::fmt;

  format_to<"#{:03d} - Out of extended memory, unable to allocate {:d} bytes.\n">(stdout, __COUNTER__, size);
  pretty_print(blist, stdout);
  std::abort();
}

[[noreturn]] 
void panick::invalid_free_ext(void* ptr, struct ::block_list const& blist) noexcept
{
  using namespace textio::fmt;
  format_to<"#{:03d} - Extended heap deallocation failed ({:08p}), possible corruption.\n">(stdout, __COUNTER__, ptr);
  pretty_print(blist, stdout);
  std::abort();
}

[[noreturn]]
void panick::pxenv_error(const char* what) noexcept
{
  using namespace textio::fmt;
  format_to<"#{:03d} - PXE error, {:s}.\n">(stdout, __COUNTER__, what);
  std::abort();
}

[[noreturn]] 
void panick::unable_to_download(const char* what) noexcept
{
  using namespace textio::fmt;
  format_to<"#{:03d} - Unable to download {:s}.\n">(stdout, __COUNTER__, what);
  std::abort();
}

namespace std
{
  [[noreturn]] 
  void __throw_bad_alloc()
  {
    using namespace textio::fmt;
    format_to<"#{:03d} - Bad allocation error.\n">(stdout, __COUNTER__);
    std::abort();
  }

  [[noreturn]]
  void __throw_bad_cast()
  {
    using namespace textio::fmt;
    format_to<"#{:03d} - Bad cast error\n">(stdout, __COUNTER__);
    std::abort();
  }  

  [[noreturn]] 
  void __throw_length_error(char const* err)
  {
    using namespace textio::fmt;
    format_to<"#{:03d} - Length error ({:s}).\n">(stdout, __COUNTER__, err);
    std::abort();
  }


  [[noreturn]] 
  void __throw_invalid_argument(char const* err)
  {    
    using namespace textio::fmt;
    format_to<"#{:03d} - Invalid argument ({:s}).\n">(stdout, __COUNTER__, err);
    std::abort();
  }

  [[noreturn]] 
  void __throw_runtime_error(char const* err) 
  {
    using namespace textio::fmt;
    format_to<"#{:03d} - Runtime error ({:s}).\n">(stdout, __COUNTER__, err);
    std::abort();
  }

  [[noreturn]]
  void __throw_logic_error(char const* err)
  {
    using namespace textio::fmt;
    format_to<"#{:03d} - Logic error ({:s}).\n">(stdout, __COUNTER__, err);
    std::abort();
  }

  [[noreturn]]
  void __throw_bad_array_new_length()
  {
    using namespace textio::fmt;
    format_to<"#{:03d} - Bad array new length.\n">(stdout, __COUNTER__);
    std::abort();
  }

  [[noreturn]]
  void __throw_out_of_range_fmt(char const*, ...)
  {
    using namespace textio::fmt;
    format_to<"#{:03d} - Out of range fmt.\n">(stdout, __COUNTER__);
    std::abort();
  }

}

