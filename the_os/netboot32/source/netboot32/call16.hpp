#pragma once

#include <hardware/x86call16.hpp>

namespace netboot32
{
  using x86arch::call16_context_type;
  void call16_fun (call16_context_type& ctx, std::uint16_t cs, std::uint16_t ip);
  void call16_int (call16_context_type& ctx, std::uint8_t num);
}