#include <bios_data_area.hpp>
#include <display.hpp>
#include <assembly.hpp>
#include <utilities.hpp>
#include <debug.hpp>

#include <cstddef>
#include <span>
#include <ranges>
#include <algorithm>

extern "C"
{
  volatile uint32_t follow_cursor_x;
  volatile uint32_t follow_cursor_y;
  volatile uint32_t follow_page_rows;
}

struct display_device
{
  std::uint16_t tab_size;
  std::uint16_t attribute;
  std::uint16_t video_io;
  std::uint16_t page_cols;
  std::uint16_t page_rows;
  std::uint8_t  cursor_x;
  std::uint8_t  cursor_y;  
  std::span<std::uint16_t> buffer;

  display_device() : 
    attribute (0x0700u),
    tab_size  (8u),
    video_io  (BDA::video_adapter_io_port),
    page_cols (BDA::number_of_columns),
    page_rows (BDA::last_row_number + 1),
    cursor_x  (BDA::page_cursor_position[BDA::active_video_page][0]),
    cursor_y  (BDA::page_cursor_position[BDA::active_video_page][1]),
    buffer    ((std::uint16_t*)(0xB8000 + BDA::offset_of_video_page), page_rows * page_cols)
  {}

  ~display_device()
  {}

  void set_attribute(std::uint8_t value)
  {
    attribute = uint16_t(value*0x100u);
  }

  void write_char(char value)
  {
    if (value >= ' ') {
      buffer[cursor_y * page_cols + cursor_x] = attribute + value;
    }
    advance_cursor(value);
  }

  void scroll_down()
  {
    using std::ranges::copy;
    using std::ranges::fill;
    copy(buffer.subspan(page_cols), buffer.begin());
    fill(buffer.subspan(page_cols * (page_rows - 1), page_cols), attribute);
  }

  void advance_line_feed()
  {
    cursor_x = 0;
    ++cursor_y;
    
    if (cursor_y >= page_rows) 
    {
      cursor_y = page_rows-1;
      scroll_down();
    }
  }

  void advance_cariage_return()
  {
    cursor_x = 0;
  }

  void advance_tabulate()
  {
    ++cursor_x;
    cursor_x = quantize_to(tab_size, cursor_x);
    if (cursor_x >= page_cols)
      advance_line_feed();
  }

  void advance_backspace()
  {
    if (cursor_x > 0)
      --cursor_x;
  }

  void advance_normal()
  {
    ++cursor_x;
    if (cursor_x >= page_cols)
      advance_line_feed();
  }

  void advance_cursor(char value)
  {
    switch(value)
    {
    case '\n': advance_line_feed(); break;      
    case '\r': advance_cariage_return(); break;
    case '\t': advance_tabulate(); break;
    case '\b': advance_backspace(); break;
    default  : advance_normal(); break;
    }
    update_hardware_cursor();
  }

  void update_hardware_cursor()
  {
    using namespace assembly;

    BDA::page_cursor_position[BDA::active_video_page][0] = cursor_x;
    BDA::page_cursor_position[BDA::active_video_page][1] = cursor_y;

    const std::uint16_t pos = BDA::offset_of_video_page + cursor_y * page_cols + cursor_x; 

    outb(video_io+0, 0x0Fu);
    outb(video_io+1, ((pos >> 0u) & 0xFF));
    outb(video_io+0, 0x0Eu);
    outb(video_io+1, ((pos >> 8u) & 0xFF));
  }
};

static display_device _display_instance;

void display(char value)
{
  _display_instance.write_char(value);
}

void display(std::string_view value)
{
  for (auto chr : value) {
    display(chr);
  }
}

void display(std::initializer_list<std::string_view> values)
{
  for (auto value : values) {
    display(value);
  }
}