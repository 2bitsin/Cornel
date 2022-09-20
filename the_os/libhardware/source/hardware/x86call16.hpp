#pragma once

#include <cstddef>
#include <cstdint>

#include <call16_thunk/call16_thunk.hpp>

namespace x86arch
{
  using x86arch::detail::call16_context_type;
  void call16_fun (call16_context_type& ctx, std::uint16_t cs, std::uint16_t ip);
  void call16_int (call16_context_type& ctx, std::uint8_t num);
}