#pragma once

#include <concepts>

namespace keyboard
{
  void initialize(bool first_time);
  void finalize(bool last_time);

  void irq() noexcept;

  bool is_pressed(std::uint16_t scancode) noexcept;

  template <std::convertible_to<std::uint16_t>... Scan_code>
  bool is_pressed_all(Scan_code... value) noexcept
  {
    return (is_pressed(value) && ...);
  }

  template <std::convertible_to<std::uint16_t>... Scan_code>
  bool is_pressed_any(Scan_code... value) noexcept
  {
    return (is_pressed(value) || ...);
  }


  namespace scancode
  {
    inline static const constexpr std::uint16_t shift      = 0x100u;
    inline static const constexpr std::uint16_t ctrl       = 0x101u;
    inline static const constexpr std::uint16_t alt        = 0x102u;

    inline static const constexpr std::uint8_t left_ctrl   = 0x00u + 0x1Du;
    inline static const constexpr std::uint8_t right_ctrl  = 0x80u + 0x1Du;

    inline static const constexpr std::uint8_t left_alt    = 0x00u + 0x38u;
    inline static const constexpr std::uint8_t right_alt   = 0x80u + 0x38u;

    inline static const constexpr std::uint8_t left_shift  = 0x00u + 0x2Au;
    inline static const constexpr std::uint8_t right_shift = 0x00u + 0x36u;

    inline static const constexpr std::uint8_t delete_key  = 0x00u + 0x53u;
  }
}