#include <cstdint>

#include <interrupts.hpp>
#include <globals.hpp>
#include <macros.hpp>
#include <display.hpp>

CO_PUBLIC 
CO_SECTION(".text.main")
auto main () -> void
{
  global_initialize();  
  display_write_string(
    "Saul Goodman, defending Tuco Salamanca in court, against a drug trafficking charge.\n"
    "\n"
    "Saul Goodman: [to Tuco Salamanca] You're being charged with drug trafficking, a very serious offense. If convicted, you could be facing many years in prison.\n"
    "\n"
    "But I believe that we can beat this charge. The key is to show the jury that you are not a drug trafficker. You are a businessman. A legitimate businessman who just happens to sell drugs.\n"
    "\n"
    "I will do everything in my power to help you prove your innocence. But you need to be honest with me. Are you ready to fight this charge?\n");

  asm("hlt");
  //global_finalize();
}
