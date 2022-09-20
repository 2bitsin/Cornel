#pragma once

#include <cstddef>
#include <hardware/x86utils.hpp>

namespace x86arch::detail
{
  extern "C"
  {
    extern const std::uint8_t* volatile call16_thunk_begin;
    extern const std::uint8_t* volatile call16_thunk_end;
    extern const std::uint8_t* volatile call16_thunk_target;
  }

  #pragma pack(push, 1)
  struct call16_context
  { 
    std::uint32_t eax;
    std::uint32_t ebx;
    std::uint32_t ecx;
    std::uint32_t edx;    
    std::uint32_t esi;
    std::uint32_t edi;
    std::uint32_t ebp;
    
    std::uint16_t ds;    
    std::uint16_t es;

    std::uint16_t fs;
    std::uint16_t gs;

    std::uint32_t esp;
    std::uint16_t ss;
    std::uint16_t flags;
  };

  using four_bytes_type = std::byte[4];

  using call16_address = x86arch::real_address;
  
  union call16_thunk_layout
  {
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wpedantic"
    std::uint8_t code[512];
    struct
    {
      four_bytes_type jump;
      call16_context regs;
      call16_address addr;
    };
  #pragma GCC diagnostic pop
  };

  #pragma pack(pop)

} // namespace name

