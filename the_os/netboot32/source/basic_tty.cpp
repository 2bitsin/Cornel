#include <bios_data_area.hpp>
#include <basic_tty.hpp>
#include <port_io.hpp>

struct basic_tty
{
  std::uint16_t page_columns;
  std::uint16_t page_rows;
  std::uint16_t cursor_x;
  std::uint16_t cursor_y;
  std::uint8_t  attribute;
  std::uint32_t page_offset;
  std::uint16_t *buffer;

  basic_tty() : 
    page_columns { BDA_number_of_columns },
    page_rows    { BDA_size_of_video_page / (page_columns * 2) },
    cursor_x     { BDA_page_cursor_position[BDA_active_video_page] >> 8 },
    cursor_y     { BDA_page_cursor_position[BDA_active_video_page] & 0xFF },
    attribute    { 0x07 },
    page_offset  { BDA_offset_of_video_page },
    buffer       { (std::uint16_t*)0xB8000 }
  {}

  ~basic_tty()
  {}
};

static basic_tty basic_tty_instance;