#include <netboot32/panick.hpp>
#include <hardware/console.hpp>
#include <memory/block_list.hpp>

#include <cstdlib>

CO_PUBLIC 
[[noreturn]]
void abort()
{
  using namespace textio::simple;
  writeln_to(stdout, "Halting system.");
  x86arch::cli();
  x86arch::hlt();
  for(;;);
}

[[noreturn]]
void panick::cant_enable_atwenty() noexcept
{
  using namespace textio::simple;
  writeln_to(stdout, "#001 - Unable to enable address line 20");
  std::abort();
}

[[noreturn]] 
void panick::out_of_memory(std::size_t size) noexcept
{
  using namespace textio::simple;
  writeln_to(stdout, "#002 - Out of memory, unable to allocate ", size, " bytes");
  std::abort();
}

[[noreturn]] 
void panick::invalid_free(void* ptr) noexcept
{
  using namespace textio::simple;
  using namespace textio::simple::fmt;
  writeln_to(stdout, "#003 - Heap deallocation failed (", hex<'&'>(ptr), "), possible corruption.");
  std::abort();
}

[[noreturn]] 
void panick::out_of_memory(std::size_t size, struct ::block_list const& blist) noexcept
{
  using namespace textio::simple;
  writeln_to(stdout, "#002.0 - Out of base memory, unable to allocate ", size, " bytes");
  pretty_print(blist, stdout);
  std::abort();
}

[[noreturn]] 
void panick::invalid_free(void* ptr, struct ::block_list const& blist) noexcept
{
  using namespace textio::simple;
  using namespace textio::simple::fmt;
  writeln_to(stdout, "#003.0 - Base heap deallocation failed (", hex<'&'>(ptr), "), possible corruption.");
  pretty_print(blist, stdout);
  std::abort();
}

[[noreturn]] 
void panick::out_of_memory_ext(std::size_t size, struct ::block_list const& blist) noexcept
{
  using namespace textio::simple;
  writeln_to(stdout, "#002.1 - Out of extended memory, unable to allocate ", size, " bytes");
  pretty_print(blist, stdout);
  std::abort();
}

[[noreturn]] 
void panick::invalid_free_ext(void* ptr, struct ::block_list const& blist) noexcept
{
  using namespace textio::simple;
  using namespace textio::simple::fmt;
  writeln_to(stdout, "#003.1 - Extended heap deallocation failed (", hex<'&'>(ptr), "), possible corruption.");
  pretty_print(blist, stdout);
  std::abort();
}

namespace std
{
  [[noreturn]] 
  void __throw_bad_alloc()
  {
    using namespace textio::simple;
    writeln_to(stdout, "#004 - Bad allocation error");
    std::abort();
  }

  [[noreturn]]
  void __throw_bad_cast()
  {
    using namespace textio::simple;
    writeln_to(stdout, "#005 - Bad cast error");
    std::abort();
  }

  [[noreturn]]

  [[noreturn]] 
  void __throw_length_error(char const* err)
  {
    using namespace textio::simple;
    writeln_to(stdout, "#006 - Length error (", err, ")");
    std::abort();
  }


  [[noreturn]] 
  void __throw_invalid_argument(char const* err)
  {    
    using namespace textio::simple;
    writeln_to(stdout, "#007 - Invalid argument (", err, ")");
    std::abort();
  }

  [[noreturn]] 
  void __throw_runtime_error(char const* err) 
  {
    using namespace textio::simple;
    writeln_to(stdout, "#008 - Runtime error (", err, ")");
    std::abort();
  }
}


[[noreturn]] 
void panick::invalid_pxenvplus() noexcept
{
  using namespace textio::simple;
  writeln_to(stdout, "#009 - Invalid PXENV+ structure");
  std::abort();
}

[[noreturn]]
void panick::invalid_bangpxe() noexcept
{
  using namespace textio::simple;
  writeln_to(stdout, "#010 - Invalid !PXE structure");
  std::abort();
}

[[noreturn]]
void panick::pxe_failed(const char* what) noexcept
{
  using namespace textio::simple;
  writeln_to(stdout, "#011 - PXE api failure: ", what);
  std::abort();
}

