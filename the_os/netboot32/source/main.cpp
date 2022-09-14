#include <cstddef>
#include <cstdint>
#include <span>
#include <charconv>
#include <vector>
#include <string>

#include <hardware/console.hpp>
#include <hardware/bios_data_area.hpp>
#include <hardware/assembly.hpp>
#include <hardware/atwenty.hpp>

#include <misc/macros.hpp>

#include <textio/simple.hpp>

#include <netboot32/interrupts.hpp>
#include <netboot32/runtime.hpp>
#include <netboot32/memory.hpp>

void initialize(bool first_time)
{
  runtime::initialize(first_time);
  isr::initialize(first_time);  
  atwenty::initialize(first_time);
  memory::initialize(first_time);
  
}

void finalize(bool last_time)
{
  memory::finalize(last_time);
  atwenty::finalize(last_time);
  isr::finalize(last_time);
  runtime::finalize(last_time);
}

CO_PUBLIC 
auto main () -> void
{
  using namespace std::string_view_literals;
  using namespace std::string_literals;
  using namespace textio::simple;
  initialize(true);
  
  std::vector<std::string> test = 
  {
    "Hello world!"s,
    "This is a test."s,
    "This is a test of the emergency broadcast system."s,
    "This is only a test."s,
    "If this had been an actual emergency, you would have been instructed to do something."s
  };
  
  for (auto&& line : test)
  {
    console::writeln(std::string_view(line));
  }
  

  for(;;) 
  {
    assembly::hlt();
    console::write("Tick!\r");
    assembly::hlt();
    console::write("Tock!\r");
  }

  finalize(true);
}
