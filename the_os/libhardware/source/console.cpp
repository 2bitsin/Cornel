#include <hardware/console.hpp>
#include <utils/int.hpp>

console::console() : 
  tab_size  (8u),
  attribute (0x0700u),
  video_io  (bda::video_adapter_io_port),
  page_cols (bda::number_of_columns),
  page_rows (bda::last_row_number + 1),
  cursor_x  (bda::page_cursor_position[bda::active_video_page][0]),
  cursor_y  (bda::page_cursor_position[bda::active_video_page][1]),
  buffer    ((std::uint16_t*)(0xB8000 + bda::offset_of_video_page), page_rows * page_cols)
{
  __debug_print(__func__);
  __debug_print("\r\n");
}

console::~console()
{
  __debug_print(__func__);
  __debug_print("\r\n");
}

void console::set_attribute(std::uint8_t value)
{
  attribute = uint16_t(value*0x100u);
}

void console::write_char(char value)
{
  __debug_char(value);
  if (value >= ' ') {    
    buffer[cursor_y * page_cols + cursor_x] = attribute + value;
  }
  advance_cursor(value);
}

void console::scroll_down()
{
  using std::ranges::copy;
  using std::ranges::fill;
  copy(buffer.subspan(page_cols), buffer.begin());
  fill(buffer.subspan(page_cols * (page_rows - 1), page_cols), attribute);
}

void console::advance_line_feed()
{
  cursor_x = 0;
  ++cursor_y;
  
  if (cursor_y >= page_rows) 
  {
    cursor_y = page_rows-1;
    scroll_down();
  }
}

void console::advance_cariage_return()
{
  cursor_x = 0;
}

void console::advance_tabulate()
{
  ++cursor_x;
  cursor_x = quantize_to(tab_size, cursor_x);
  if (cursor_x >= page_cols)
    advance_line_feed();
}

void console::advance_backspace()
{
  if (cursor_x > 0)
    --cursor_x;
}

void console::advance_normal()
{
  ++cursor_x;
  if (cursor_x >= page_cols)
    advance_line_feed();
}

void console::advance_cursor(char value)
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

void console::update_hardware_cursor()
{
  using namespace x86arch;
  const std::uint16_t pos = bda::offset_of_video_page + cursor_y * page_cols + cursor_x; 
  bda::page_cursor_position[bda::active_video_page][0] = cursor_x;
  bda::page_cursor_position[bda::active_video_page][1] = cursor_y;
  outb(video_io+0, 0x0Fu);
  outb(video_io+1, ((pos >> 0u) & 0xFF));
  outb(video_io+0, 0x0Eu);
  outb(video_io+1, ((pos >> 8u) & 0xFF));
}

auto console::instance() -> console&
{
  static console _;
  return _;
}