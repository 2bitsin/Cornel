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

#include <script/interpreter.hpp>
#include <script/executor.hpp>

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

struct cmd_say: script::command_base<cmd_say, "say", std::vector<std::string>>
{
  auto execute(auto&& context_v, std::vector<std::string> const& args_v) -> int
  {
    using namespace textio::fmt;
    for(auto const& arg: args_v)
      format_to<"{} ">(context_v.stdout_handle(), arg);
    format_to<"\n">(context_v.stdout_handle());
    return 0;
  }
};

struct netboot_executor: script::executor<netboot_executor, cmd_say>
{

};

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

  script::interpreter si;
  netboot_executor se;

  si.execute(se, std::string_view{buffer});


  asm("int $0x3");

  pxenv::finalize(false);
  finalize(true);
}
