#pragma once

#include <span>
#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <ranges>
#include <iterator>

#include <hardware/bios_data_area.hpp>
#include <hardware/assembly.hpp>
#include <hardware/utilities.hpp>
#include <hardware/debug.hpp>

struct vga_text
{
  vga_text();
  ~vga_text();

  void set_attribute(std::uint8_t value);
  void write_char(char value);

  struct iterator
  {
    using iterator_category = std::output_iterator_tag;
    using value_type        = void;
    using pointer           = void;
    using reference         = void;

#ifdef __cpp_lib_concepts
    using difference_type = ptrdiff_t;
#else
    using difference_type = void;
#endif // __cpp_lib_concepts

    iterator& operator=(const char& val) {
        vga_text::instance().write_char(val);
        return *this;
    }

    iterator& operator=(char&& val) {
        vga_text::instance().write_char(val);
        return *this;
    }

    iterator& operator*() noexcept{
        return *this;
    }

    iterator& operator++() noexcept{
        return *this;
    }

    iterator operator++(int) noexcept{
        return *this;
    }
  }; 

protected:
  void scroll_down();
  void advance_line_feed();
  void advance_cariage_return();
  void advance_tabulate();
  void advance_backspace();
  void advance_normal();
  void advance_cursor(char value);
  void update_hardware_cursor();

  static auto instance() -> vga_text&;

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

static inline auto tty() 
{
  return vga_text::iterator{};
}
