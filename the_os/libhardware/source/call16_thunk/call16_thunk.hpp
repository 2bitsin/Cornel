#pragma once

#include <cstddef>
#include <hardware/x86real_addr.hpp>

namespace x86arch::detail
{
  extern "C"
  {
    extern const std::uint8_t* volatile call16_thunk_begin;
    extern const std::uint8_t* volatile call16_thunk_end;
    extern const std::uint8_t* volatile call16_thunk_target;
  }

  #pragma pack(push, 1)
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wpedantic"
  
  struct call16_context
  { 
    std::uint16_t irq_mask;

    union { std::uint32_t eax; std::uint16_t ax; struct { std::uint8_t al, ah; }; }; 
    union { std::uint32_t ebx; std::uint16_t bx; struct { std::uint8_t bl, bh; }; }; 
    union { std::uint32_t ecx; std::uint16_t cx; struct { std::uint8_t cl, ch; }; }; 
    union { std::uint32_t edx; std::uint16_t dx; struct { std::uint8_t dl, dh; }; };
    union { std::uint32_t esi; std::uint16_t si; };
    union { std::uint32_t edi; std::uint16_t di; };
    union { std::uint32_t ebp; std::uint16_t bp; };
      
    std::uint16_t ds;    
    std::uint16_t es;
    std::uint16_t fs;
    std::uint16_t gs;

    union { std::uint32_t esp; std::uint16_t sp; };

    std::uint16_t ss;
    std::uint16_t flags;    
  };

  using call16_address = x86arch::real_address;
  
  union call16_thunk_layout
  {
    std::uint8_t code[512];
    struct
    {
      std::byte jump[2];
      call16_context regs;
      call16_address addr;
    };
  };

  #pragma GCC diagnostic pop
  #pragma pack(pop)

} // namespace name

