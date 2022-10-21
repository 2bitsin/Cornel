#include <cstdint>
#include <cstddef>
#include <ranges>
#include <algorithm>

#include <textio/simple.hpp>
#include <textio/simple/fmt.hpp>

#include <netboot32/keyboard.hpp>
#include <hardware/kbdctrl.hpp>
#include <hardware/console.hpp>

namespace keyboard
{
  static volatile std::uint8_t G_last_scancode = 0x00u;
  static volatile bool G_keyboard_state [256];
    
  bool is_pressed(std::uint16_t value) noexcept
  {
    switch (value)
    {
    case scancode::shift: 
      { return is_pressed_any(scancode::left_shift, scancode::right_shift); }
    case scancode::ctrl:
      { return is_pressed_any(scancode::left_ctrl, scancode::right_ctrl); }
    case scancode::alt:
      { return is_pressed_any(scancode::left_alt, scancode::right_alt); }
    default:
      value &= 0x00FFu;
      break;
    }    
    return G_keyboard_state[value];
  }

  void irq() noexcept
  {
    auto curr_scancode = kbdctrl::read_data(kbdctrl::dont_wait);
    if (curr_scancode != 0xE0u) 
    { G_keyboard_state[curr_scancode & 0x7Fu] = !(curr_scancode & 0x80u); }
    G_last_scancode = curr_scancode;

    using namespace scancode;
    if (is_pressed_all(ctrl, alt, delete_key))    
    { kbdctrl::hardware_reset(); }
  }

  void initialize(bool first_time)
  {
    if (!first_time)
      return;   

    std::ranges::fill(G_keyboard_state, false);
  }

  void finalize(bool last_time)
  {
    if (!last_time)
      return;       
  }
}

