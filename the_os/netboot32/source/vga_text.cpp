#include <print.hpp>

#include "vga_text.hpp"

vga_text::vga_text() : 
  attribute (0x0700u),
  tab_size  (8u),
  video_io  (BDA::video_adapter_io_port),
  page_cols (BDA::number_of_columns),
  page_rows (BDA::last_row_number + 1),
  cursor_x  (BDA::page_cursor_position[BDA::active_video_page][0]),
  cursor_y  (BDA::page_cursor_position[BDA::active_video_page][1]),
  buffer    ((std::uint16_t*)(0xB8000 + BDA::offset_of_video_page), page_rows * page_cols)
{}

vga_text::~vga_text()
{}

void vga_text::set_attribute(std::uint8_t value)
{
  attribute = uint16_t(value*0x100u);
}

void vga_text::write_char(char value)
{
  if (value >= ' ') {
    buffer[cursor_y * page_cols + cursor_x] = attribute + value;
  }
  advance_cursor(value);
}

void vga_text::scroll_down()
{
  using std::ranges::copy;
  using std::ranges::fill;
  copy(buffer.subspan(page_cols), buffer.begin());
  fill(buffer.subspan(page_cols * (page_rows - 1), page_cols), attribute);
}

void vga_text::advance_line_feed()
{
  cursor_x = 0;
  ++cursor_y;
  
  if (cursor_y >= page_rows) 
  {
    cursor_y = page_rows-1;
    scroll_down();
  }
}

void vga_text::advance_cariage_return()
{
  cursor_x = 0;
}

void vga_text::advance_tabulate()
{
  ++cursor_x;
  cursor_x = quantize_to(tab_size, cursor_x);
  if (cursor_x >= page_cols)
    advance_line_feed();
}

void vga_text::advance_backspace()
{
  if (cursor_x > 0)
    --cursor_x;
}

void vga_text::advance_normal()
{
  ++cursor_x;
  if (cursor_x >= page_cols)
    advance_line_feed();
}

void vga_text::advance_cursor(char value)
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

void vga_text::update_hardware_cursor()
{
  using namespace assembly;
  const std::uint16_t pos = BDA::offset_of_video_page + cursor_y * page_cols + cursor_x; 
  BDA::page_cursor_position[BDA::active_video_page][0] = cursor_x;
  BDA::page_cursor_position[BDA::active_video_page][1] = cursor_y;
  outb(video_io+0, 0x0Fu);
  outb(video_io+1, ((pos >> 0u) & 0xFF));
  outb(video_io+0, 0x0Eu);
  outb(video_io+1, ((pos >> 8u) & 0xFF));
}


static vga_text print_device;
static struct vga_text_attach
{
  static void output(char value)
  {
    print_device.write_char(value);
  }

  vga_text_attach()
  {
    print_attach_output(&vga_text_attach::output);
  }

  ~vga_text_attach() 
  {
    print_detach_output(&vga_text_attach::output);
  }
} _attach;
