#include <cstdint>
#include <cstddef>
#include <cstdlib>

#include <algorithm>
#include <span>

#include <utils/macros.hpp>
#include <utils/debug.hpp>

#include <hardware/x86assembly.hpp>
#include <hardware/console.hpp>
#include <hardware/bios_data_area.hpp>

#include <netboot32/runtime.hpp>

#include <memory/block_list.hpp>


using ctor_fun_t = void(void);
using ctor_ptr_t = ctor_fun_t*;

using dtor_fun_t = void(void*);
using dtor_ptr_t = dtor_fun_t*;

extern "C"
{  
  ctor_ptr_t* volatile G_init_array_begin;
  ctor_ptr_t* volatile G_init_array_end;

  std::byte* volatile G_bss_begin;
  std::byte* volatile G_bss_end;

}

auto runtime::initialize(bool first_time) -> void
{  
  using std::span;

  if (!first_time)
    return;
  
  // Zero out .bss section
  std::ranges::fill(span{ G_bss_begin, G_bss_end }, std::byte());  

  // Call all constructors
  std::ranges::for_each(span{ G_init_array_begin, G_init_array_end }, [] (auto&& ctor) { ctor(); });
}


auto runtime::finalize([[maybe_unused]] bool last_time) -> void
{}

  /* The ABI requires a 64-bit type.  */
__extension__ typedef int __guard __attribute__((mode(__DI__)));   

CO_PUBLIC void __cxa_atexit         (void (*)(void*), void*, void*) {}
CO_PUBLIC void __cxa_finalize       (void*) {}
CO_PUBLIC int  __cxa_guard_acquire  (__guard *g) { return !*(char *)(g); } 
CO_PUBLIC void __cxa_guard_release  (__guard *g) { *(char *)g = 1; } 
CO_PUBLIC void __cxa_guard_abort    (__guard *)  {}

void* __dso_handle = nullptr;

