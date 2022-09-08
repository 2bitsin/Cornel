#pragma once

#include <span>
#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <ranges>
#include <iterator>

#include <hardware/bios_data_area.hpp>
#include <hardware/assembly.hpp>
#include <misc/debug.hpp>
#include <textio/simple.hpp>

struct console
{
  console();
  ~console();

  void set_attribute(std::uint8_t value);
  void write_char(char value);

  struct iterator
  {
    using iterator_category = std::output_iterator_tag;
    using value_type        = void;
    using pointer           = void;
    using reference         = void;
    using difference_type   = ptrdiff_t;
    iterator& operator = (const char& val) { console::instance().write_char(val); return *this; }
    iterator& operator = (char&& val) { console::instance().write_char(val); return *this; }
    iterator& operator * () noexcept { return *this; }
    iterator& operator ++ () noexcept { return *this; }
    iterator operator ++ (int) noexcept { return *this; }
  }; 

  template <typename... T> static inline auto write   (T&&... args) { return textio::simple::write   (iterator{}, std::forward<T>(args)...); }
  template <typename... T> static inline auto writeln (T&&... args) { return textio::simple::writeln (iterator{}, std::forward<T>(args)...); }

protected:
  void scroll_down();
  void advance_line_feed();
  void advance_cariage_return();
  void advance_tabulate();
  void advance_backspace();
  void advance_normal();
  void advance_cursor(char value);
  void update_hardware_cursor();

  static auto instance() -> console&;

  friend struct iterator;  

private:
  std::uint16_t tab_size;
  std::uint16_t attribute;
  std::uint16_t video_io;
  std::uint16_t page_cols;
  std::uint16_t page_rows;
  std::uint8_t  cursor_x;
  std::uint8_t  cursor_y;  
  std::span<std::uint16_t> buffer;
};
