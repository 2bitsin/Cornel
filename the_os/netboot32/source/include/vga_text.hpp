#pragma once

#include <span>
#include <cstddef>
#include <cstdint>
#include <algorithm>
#include <ranges>

#include <bios_data_area.hpp>
#include <assembly.hpp>
#include <utilities.hpp>
#include <debug.hpp>

struct vga_text
{
  vga_text();
  ~vga_text();

  void set_attribute(std::uint8_t value);
  void write_char(char value);

protected:
  void scroll_down();
  void advance_line_feed();
  void advance_cariage_return();
  void advance_tabulate();
  void advance_backspace();
  void advance_normal();
  void advance_cursor(char value);
  void update_hardware_cursor();

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
