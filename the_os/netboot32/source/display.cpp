#include <bios_data_area.hpp>
#include <display.hpp>
#include <assembly.hpp>
#include <utilities.hpp>

#include <cstddef>
#include <span>
#include <ranges>
#include <algorithm>

struct display
{
  std::uint16_t tab_size;
  std::uint16_t attribute;
  std::uint16_t page_cols;
  std::uint16_t page_rows;
  std::uint16_t cursor_x;
  std::uint16_t cursor_y;  
  std::span<std::uint16_t> buffer;

  display() : 
    attribute (0x0700u),
    tab_size  (8u),
    page_cols (BDA_number_of_columns),
    page_rows (BDA_size_of_video_page / (BDA_number_of_columns * 2)),
    cursor_x  (BDA_page_cursor_position[BDA_active_video_page] >> 8),
    cursor_y  (BDA_page_cursor_position[BDA_active_video_page] & 0xFF),
    buffer    ((std::uint16_t*)(0xB8000 + BDA_offset_of_video_page), page_rows * page_cols)
  {}

  ~display()
  {}

  void set_attribute(std::uint8_t value)
  {
    attribute = uint16_t(value*0x100u);
  }

  void write_char(char value)
  {
    buffer[cursor_y * page_cols + cursor_x] = attribute + value;
    advance_cursor(value);
  }

  void scroll_down()
  {
    using std::ranges::copy;
    copy(buffer.subspan(page_cols), buffer.begin());
  }

  void advance_cursor(char value)
  {
    switch(value)
    {
    case '\n':
      {
        cursor_x = 0;
        ++cursor_y;
        if (cursor_y >= page_rows)
          cursor_y = page_rows-1;
        scroll_down();
        break;
      }
    case '\r': 
      { 
        cursor_x = 0;
        break;
      }
    case '\t': 
      {
        ++cursor_x;
        cursor_x = quantize_to(tab_size, cursor_x);
        if (cursor_x >= page_cols)
          advance_cursor('\n');          
        break;
      }
    case '\b':
      {
        if (cursor_x > 0)
          --cursor_x;
        break;
      }
    default:
      {
        ++cursor_x;
        if (cursor_x >= page_cols)
          advance_cursor('\n');
        break;
      }      
    }
  }
};

static display _display_instance;

void display_write_char(char value)
{
  _display_instance.write_char(value);
}

void display_write_string(std::string_view value)
{
  for (auto c : value)
    display_write_char(c);
}