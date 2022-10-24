#include <cstddef>
#include <cstdint>
#include <span>
#include <charconv>
#include <vector>
#include <string>

#include <netboot/interrupts.hpp>
#include <netboot/runtime.hpp>
#include <netboot/memory.hpp>
#include <netboot/progress.hpp>
#include <netboot/panick.hpp>

#include <hardware/bios_data_area.hpp>
#include <hardware/x86bios.hpp>
#include <hardware/x86assembly.hpp>

#include <utils/macros.hpp>

#include <textio/format.hpp>
#include <textio/format/helpers/repeat_value.hpp>

#include <pxenv/core.hpp>
#include <pxenv/tftp.hpp>

void initialize(bool first_time)
{
  runtime::initialize(first_time);
  interrupts::initialize(first_time);    
  memory::initialize(first_time);
  pxenv::initialize(first_time);
}

void finalize(bool last_time)
{
  pxenv::finalize(last_time);
  memory::finalize(last_time);  
  interrupts::finalize(last_time);
  runtime::finalize(last_time);
}

CO_PUBLIC 
auto main (PXENVplus&, bangPXE&) -> void
{    
  using namespace textio::fmt;
  initialize(true);

  format_to<"Starting OS ...\n">(stdout);
  progress_notify p;
  auto [status, buffer] = pxenv::tftp::download("netboot32.run", p);
  if (pxenv::pxenv_status::success != status) {
    panick::unable_to_download("netboot32.run");    
  }

  format_to(stdout, std::string_view{ buffer });

  asm("int $0x3");

  pxenv::finalize(false);
  finalize(true);
}
