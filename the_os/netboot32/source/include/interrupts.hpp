#pragma once

#include <cstddef>
#include <cstdint>

#pragma pack(push, 1)
  struct ISR_stack_frame 
  {
    std::uint32_t 
      ss, gs, fs, es, ds, 
      edi, esi, 
      edx, ecx, ebx, eax, 
      ebp, esp, 
      which, errcode, 
      eip, cs, eflags;
      
    struct 
    {
      // these are pushed only in certain situations 
      std::uint32_t esp, ss, es, ds, fs, gs;
    } x; // TODO: give it a better name       
  };
#pragma pack(pop)

struct isr
{
  static void initialize();
  static void finalize();
};