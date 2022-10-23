#include <bits/functexcept.h>
#include <new>

#include <hardware/x86address16.hpp>

auto x86arch::address16::from(void const* value) noexcept -> x86arch::address16
{ 
  using namespace std;

  if ((std::uintptr_t)value >= 0x100000u)
  { std::__throw_invalid_argument("address16::from_pointer: pointer >= 0x100000"); }

  return address16 { .off = (uint16_t)(((uintptr_t)value) & 0xfu), 
                        .seg = (uint16_t)(((uintptr_t)value) >> 4u) };
}

auto x86arch::address16::to_pointer() const noexcept -> void*
{ 
  return (void*)(seg*0x10u + off); 
}
