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

  basic_tty()
  {
    
  }

  ~basic_tty()
  {
    
  }
};

static basic_tty basic_tty_instance;