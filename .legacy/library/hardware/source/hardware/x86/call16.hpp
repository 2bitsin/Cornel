#pragma once

#include <cstddef>
#include <cstdint>

#include <call16_thunk.hpp>

namespace x86arch
{
  using call16_context = x86arch::detail::call16_context;
  using call16_address = x86arch::detail::call16_address;

  std::uint32_t call16_invoke (call16_context& ctx, call16_address const& address);
  std::uint32_t call16_invoke (call16_context& ctx, std::uint8_t number);
}

#include <hardware/x86/call16_stack.hpp>
