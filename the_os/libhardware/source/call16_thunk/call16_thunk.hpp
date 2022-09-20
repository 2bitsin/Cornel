#pragma once

namespace x86arch::detail
{
  extern "C"
  {
    extern const std::uint8_t* volatile call16_thunk_begin;
    extern const std::uint8_t* volatile call16_thunk_end;
    extern const std::uint8_t* volatile call16_thunk_target;
  }

  #pragma pack(push, 1)
  struct call16_context_type
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

  struct call16_target_type
  {
    std::uint16_t ip;
    std::uint16_t cs;
  };

  using four_bytes_type = std::byte[4];
  
  union call16_thunk_layout
  {
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wpedantic"
    std::uint8_t code[512];
    struct
    {
      four_bytes_type     jump;
      call16_context_type regs;
      call16_target_type  addr;
    };
  #pragma GCC diagnostic pop
  };

  #pragma pack(pop)

} // namespace name

