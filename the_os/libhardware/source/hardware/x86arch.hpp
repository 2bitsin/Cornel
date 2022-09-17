#pragma once

#include <hardware/assembly.hpp>
#include <hardware/x86arch.hpp>
#include <misc/macros.hpp>

namespace x86arch
{

  enum class segment_type
  {
    code,
    data,
    stack    
  };

  struct make_32bit_segment_t  
  {
    segment_type  type        { segment_type::data };
    std::uint32_t base        { 0x00000000u };
    std::uint32_t size        { 0xffffffffu };    
    std::uint8_t  priv_level  { 0x00u };
    bool          present     { true };    
  };

  auto make_32bit_segment(make_32bit_segment_t params) -> std::uint64_t;
  
}