#include <algorithm>
#include <span>

#include <globals.hpp>
#include <macros.hpp>

using ctor_fun_t = void(void);
using ctor_ptr_t = ctor_fun_t*;

extern "C"
{  
  ctor_ptr_t* G_init_array_begin;
  std::size_t G_init_array_size;  
}

void global_initialize()
{
  std::span init_array (G_init_array_begin, G_init_array_size);

  for(auto&& ctor_fun: init_array) 
  {
    ctor_fun();
  }
}

void* __dso_handle = nullptr;

CO_PUBLIC
void __cxa_atexit(void (*destructor)(void*), void* arg, void* dso_handle)
{}

CO_PUBLIC
void __cxa_finalize(void* dso_handle)
{}
