#pragma once

#include <cstddef>
#include <cstdint>

#pragma pack(push, 1)
  struct ISR_stack_frame 
  {
    std::uint32_t eax;
    std::uint32_t ebx;
    std::uint32_t ecx;
    std::uint32_t edx;
    std::uint32_t edi;
    std::uint32_t esi;
    std::uint32_t ebp;
    std::uint32_t esp;
    std::uint32_t ds;
    std::uint32_t es;
    std::uint32_t fs;
    std::uint32_t gs;
    std::uint32_t ss;
    std::uint32_t eflags;
    std::uint32_t interrupt_number;
  };
#pragma pack(pop)

