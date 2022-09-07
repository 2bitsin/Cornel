#include <cstdint>
#include <cstddef>
#include <algorithm>
#include <span>

#include <netboot32/runtime.hpp>
#include <hardware/macros.hpp>

using ctor_fun_t = void(void);
using ctor_ptr_t = ctor_fun_t*;

using dtor_fun_t = void(void*);
using dtor_ptr_t = dtor_fun_t*;

extern "C"
{  
  volatile ctor_ptr_t* G_init_array_begin;
  volatile ctor_ptr_t* G_init_array_end;

  volatile std::uint8_t* G_bss_begin;
  volatile std::uint8_t* G_bss_end;
}

auto runtime::initialize() -> void
{  
  std::span bss { G_bss_begin, G_bss_end };
  std::span init_array { G_init_array_begin, G_init_array_end };

  // Zero out .bss section
  std::ranges::fill(bss, 0);

  // Call all constructors
  std::ranges::for_each(init_array, 
    [] (auto&& ctor) { ctor(); });
}

auto runtime::finalize() -> void
{}


CO_PUBLIC
void __cxa_atexit(void (*dtor)(void*), void* arg, void* dso_handle)
{}

CO_PUBLIC
void __cxa_finalize(void* dtor)
{}

 
  /* The ABI requires a 64-bit type.  */
__extension__ typedef int __guard __attribute__((mode(__DI__)));   
CO_PUBLIC int  __cxa_guard_acquire (__guard *g) { return !*(char *)(g); } 
CO_PUBLIC void __cxa_guard_release (__guard *g) { *(char *)g = 1; } 
CO_PUBLIC void __cxa_guard_abort   (__guard *)  {}

void* __dso_handle = nullptr;