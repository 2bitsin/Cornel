#include <bits/functexcept.h>
#include <new>

#include <hardware/real_address.hpp>

auto x86arch::real_address::from(void const* value) noexcept -> x86arch::real_address
{ 
  using namespace std;

  if ((std::uintptr_t)value >= 0x100000u)
  { std::__throw_invalid_argument("real_address::from_pointer: pointer >= 0x100000"); }

  return real_address { .off = (uint16_t)(((uintptr_t)value) & 0xfu), 
                        .seg = (uint16_t)(((uintptr_t)value) >> 4u) };
}

auto x86arch::real_address::to_pointer() const noexcept -> void*
{ 
  return (void*)(seg*0x10u + off); 
}
