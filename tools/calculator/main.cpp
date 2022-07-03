#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cstddef>
#include <cstdio>

#include "common/arguments.hpp"

int main(int argc, char** argv)
{  
  arguments s_args{ argc, argv };

  if (s_args.has ("--Gdt", "--Ldt"))
  {
    uint32_t  segment_base      { 0 };
    uint64_t  segment_limit     { 0 };
    unsigned  priviledge_level  { 0 };
    bool      is_present        { false };
    bool      is_not_system     { false };
    bool      is_executable     { false };
    bool      dir_or_conf       { false };
    bool      read_write        { false };
    bool      is_accessed       { false };
    bool      granularity_4k    { false };
    bool      is_32bit          { true };

    if (s_args.has ("--base"))
      s_args.value (segment_base, "--base");
    if (s_args.has ("--limit"))
      s_args.value (segment_limit, "--limit");
    is_present = s_args.has ("--present");
    
    using namespace std::string_literals;
    using namespace std::string_view_literals;

    auto seg_type = s_args.value_or("--type", "data"s);
    if ((seg_type == "code") || (seg_type == "data") || (seg_type == "stack"))
    {
      is_not_system = true;
      if (seg_type == "code")
      {
        is_executable = true;
        dir_or_conf = s_args.has ("--conforming");
        read_write = s_args.has("--readable");
        if (s_args.has("--writable"))
          throw std::logic_error("--writable is not supported for code segments");        
      }
      else if (seg_type == "data" || seg_type == "stack")
      {
        is_executable = false;
        dir_or_conf = seg_type == "stack";
        read_write = s_args.has("--writable");
        if (s_args.has("--readable"))
          throw std::logic_error("--readable is not supported for data segments");        
      }
      priviledge_level = s_args.value_or("--privilege", 0u);
      if (priviledge_level > 3)
        throw std::logic_error("--privilege must be 0, 1, 2 or 3");
      is_accessed = s_args.has ("--accessed");     
      is_32bit = s_args.has ("--32bit");   

      std::uint64_t descriptor { 0u }; 
      if (segment_limit > 0xffffffffu)
      {
        std::cout << "WARNING: Segment limit is too large, clamping to 4GiB." << std::endl;
        segment_limit = 0xffffffffu;
      }

      if (segment_limit > 0xFFFFFu)
      {         
        std::cout << "WARNING: Segment limit is greater than 1MB, setting granularity to 4KiB and truncating last 12 bits." << std::endl;
        segment_limit >>= 12u;
        granularity_4k = true;
      }

      if (seg_type == "code" || seg_type == "data" || seg_type == "stack")
      {
        descriptor |= (segment_limit & 0xffffull);
        descriptor |= (segment_base & 0xffffffull) << 16u;
        descriptor |= (is_accessed ? 1ull : 0ull) << 40u;
        descriptor |= (read_write ? 1ull : 0ull) << 41u;
        descriptor |= (dir_or_conf ? 1ull : 0ull) << 42u;
        descriptor |= (is_executable ? 1ull : 0ull) << 43u;
        descriptor |= (is_not_system ? 1ull : 0ull) << 44u;      
        descriptor |= (priviledge_level & 3ull) << 45u;
        descriptor |= (is_present ? 1ull : 0ull) << 47u;
        descriptor |= ((segment_limit >> 16ull) & 0xfull) << 48u;
        descriptor |= (is_32bit ? 1ull : 0ull) << 54u;
        descriptor |= (granularity_4k ? 1ull : 0ull) << 55u;
        descriptor |= (segment_base >> 24ull) & 0xffull;
        std::printf("DESCRIPTOR = 0x%016lX\n", descriptor);
      }
    }
    else if ((seg_type == "tss") || (seg_type == "ldt"))
    {
      throw std::runtime_error("Currently not supported : ldt and tss segments");
    }
    else
    {
      throw std::logic_error("invalid segment type, must be either of : code, data, stack, tss, ldt");    
    }

  }
  else
  {
    throw std::logic_error("--Gdt or --Ldt must be specified");    
  }

  return 0;
}