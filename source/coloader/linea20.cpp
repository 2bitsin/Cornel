#include "linea20.hpp"
#include "inlasm.hpp"
#include "utility.hpp"

const ubyte KBCTRL_COMMAND_PORT         = 0x64;
const ubyte KBCTRL_STATUS_PORT          = 0x64;
const ubyte KBCTRL_DATA_PORT            = 0x60;
const ubyte KBCTRL_DISABLE_KEYBOARD     = 0xAD;
const ubyte KBCTRL_ENABLE_KEYBOARD      = 0xAE;
const ubyte KBCTRL_READ_OUTPUT_PORT     = 0xD0;
const ubyte KBCTRL_WRITE_OUTPUT_PORT    = 0xD1;
const ubyte KBCTRL_OUTPUT_PORT_A20_BIT  = 0x02;
const ubyte KBCTRL_OUTPUT_BUFFER_BIT    = 0x01;
const ubyte KBCTRL_INPUT_BUFFER_BIT     = 0x02;


static inline void kbctrl_wait_for_read()
{
  while(!(io_read_b(KBCTRL_STATUS_PORT) & KBCTRL_OUTPUT_BUFFER_BIT));
}

static inline void kbctrl_wait_for_write()
{
  while(io_read_b(KBCTRL_STATUS_PORT) & KBCTRL_INPUT_BUFFER_BIT);
}

static inline void kbctrl_write_cmd(ubyte cmd)
{
  kbctrl_wait_for_write();
  io_write_b(KBCTRL_COMMAND_PORT, cmd);
}

static inline void kbctrl_write_data(ubyte data)
{
  kbctrl_wait_for_write();
  io_write_b(KBCTRL_DATA_PORT, data);
}

static inline ubyte kbctrl_read_data()
{
  kbctrl_wait_for_read();
  return io_read_b(KBCTRL_DATA_PORT);
}

static void kbctrl_a20_enable(bool should_enable)
{
  irq_disable();
  kbctrl_write_cmd(KBCTRL_DISABLE_KEYBOARD);
  kbctrl_write_cmd(KBCTRL_READ_OUTPUT_PORT);
  ubyte what = kbctrl_read_data();
  kbctrl_write_cmd(KBCTRL_WRITE_OUTPUT_PORT);
  if (should_enable)
    kbctrl_write_data(what | KBCTRL_OUTPUT_PORT_A20_BIT);
  else
    kbctrl_write_data(what & ~KBCTRL_OUTPUT_PORT_A20_BIT);
  kbctrl_write_cmd(KBCTRL_ENABLE_KEYBOARD);
  kbctrl_wait_for_write();  
  irq_enable();  
}

void line_a20_enable()
{
  //TODO: Add more A20 control fallbacks
  kbctrl_a20_enable(true);
}

void line_a20_disable()
{
  //TODO: Add more A20 control fallbacks
  kbctrl_a20_enable(false);
}

bool line_a20_is_enabled()
{
  irq_disable();
  mem_write<uword>(0x0000, 0x0500, 0x55aa);  
  mem_write<uword>(0xFFFF, 0x0510, ~0x55aa);
  uword result = mem_read<uword>(0x0000, 0x0500);
  irq_enable();
  return (result == 0x55aa);    
}