#pragma once

#include <cstddef>
#include <cstdint>

#include <call16_thunk/call16_thunk.hpp>

namespace x86arch
{
  using call16_context = x86arch::detail::call16_context;
  using call16_address = x86arch::detail::call16_address;

  void call16_invoke (call16_context& ctx, call16_address const& address);
  void call16_invoke (call16_context& ctx, std::uint8_t number);

  void call16_stack_allocate(call16_context& ctx, std::uint16_t size);
  void call16_stack_deallocate(call16_context& ctx);
}