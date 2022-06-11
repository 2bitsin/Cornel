/////////////////////////////
/// Entry point
/////////////////////////////

#include "inlasm.hpp"
#include "bios.hpp"
#include "linea20.hpp"
#include "memory.hpp"

extern "C" 
{
void __cdecl coloader_main()
{
  tty_put_string("Starting Cornel DOS ...\n");

  // Initialize A20
  if (!line_a20_is_enabled ())
  {
    tty_put_string("Address line A20 is disabled.\n");
    tty_put_string("Trying to enable address line A20 ...\n");  
    line_a20_enable();    
    if (!line_a20_is_enabled ())
    {
      tty_put_string("Line A20 is still disabled.\n");    
      tty_put_string("Halting startup.\n");
      tty_put_string("Press any key to reboot...\n");
      tty_wait_key_char();
      return;
    }
     tty_put_string("Line A20 is enabled, continuing...\n");
  }
 
  memory_load_map();

  irq_enable();
  for(;;) 
  {
    tty_put_string("    TICK\r");
    irq_wait();
    tty_put_string("tock    \r");
    irq_wait();
  }
}
}