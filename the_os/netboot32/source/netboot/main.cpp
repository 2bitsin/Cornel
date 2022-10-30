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

#include <hardware/x86/bios.hpp>
#include <hardware/x86/assembly.hpp>

#include <utils/macros.hpp>

#include <textio/format.hpp>
#include <textio/format/helpers/repeat_value.hpp>
#include <textio/logger.hpp>

#include <pxenv/core.hpp>
#include <pxenv/tftp.hpp>

#include <script/interpreter.hpp>
#include <script/executor.hpp>

declare_module(Netboot);

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
auto main () -> void
{    
  using namespace textio::fmt;
  initialize(true);

  textio::logger_base::level(textio::logger_base::level_type::all);

  Gmod.info<"Starting OS ..."> ();

  progress_notify p;
  auto [status, buffer] = pxenv::tftp::download("netboot32.run", p);
  if (pxenv::pxenv_status::success != status) {
    std::abort();
  }

  //script::interpreter si;
  //netboot_executor se;
  //si.execute(se, std::string_view{buffer});
  asm("int $0x3");

  finalize(true);
}
