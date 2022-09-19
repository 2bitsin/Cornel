#pragma once

namespace x86arch
{
  #pragma pack(push, 1)
  struct call_context_type
  { 
    std::uint16_t ip;
    std::uint16_t cs;

    std::uint16_t ds;    
    std::uint16_t es;

    std::uint16_t fs;
    std::uint16_t gs;

    std::uint16_t ss;
    std::uint16_t flags; 

    std::uint32_t eax;
    std::uint32_t ebx;
    std::uint32_t ecx;
    std::uint32_t edx;
    std::uint32_t esi;
    std::uint32_t edi;
    std::uint32_t ebp;
    std::uint32_t esp;
  };
  #pragma pack(pop)

  void call_fun16(call_context_type& ctx);
  void call_int16(call_context_type& ctx, std::uint8_t num);
}