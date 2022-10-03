#pragma once

#include <cstddef>
#include <cstdint>

namespace interrupts
{
#define A4 __attribute__((aligned(4)))
#define A1 __attribute__((aligned(1)))

  struct stack_frame 
  {
    A4 std::uint16_t ss;
    A4 std::uint16_t gs;
    A4 std::uint16_t fs;
    A4 std::uint16_t es;
    A4 std::uint16_t ds;

    A4 std::uint32_t edi;
    A4 std::uint32_t esi;
    A4 std::uint32_t edx;
    A4 std::uint32_t ecx;
    A4 std::uint32_t ebx;
    A4 std::uint32_t eax;
    A4 std::uint32_t ebp; 
    A4 std::uint32_t esp; 
    A4 std::uint32_t which;
    A4 std::uint32_t errcode; 
    A4 std::uint32_t eip; 
    A4 std::uint16_t cs;
    A4 std::uint32_t eflags;
      
    A1 struct 
    {
      // these are pushed only in certain situations 
      A4 std::uint32_t esp;

      A4 std::uint16_t ss;
      A4 std::uint16_t es;
      A4 std::uint16_t ds;
      A4 std::uint16_t fs;
      A4 std::uint16_t gs;
    } x; // TODO: give it a better name       
  };

#undef A1
#undef A4

  auto initialize(bool first_time) -> void;
  auto finalize(bool last_time) -> void;
  auto default_interrupt_mask() -> std::uint16_t;
};