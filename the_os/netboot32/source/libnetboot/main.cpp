#include <cstddef>
#include <cstdint>
#include <span>
#include <charconv>
#include <vector>
#include <string>

#include <netboot/interrupts.hpp>
#include <netboot/runtime.hpp>
#include <netboot/memory.hpp>

#include <hardware/bios_data_area.hpp>
#include <hardware/x86bios.hpp>
#include <hardware/x86assembly.hpp>

#include <utils/macros.hpp>

#include <textio/format.hpp>

#include <pxenv/core.hpp>
#include <pxenv/tftp.hpp>

struct print_progress: ::pxenv::tftp::Inotify
{
  print_progress() = default;

  bool initialize (std::string_view& name, ::pxenv::tftp::params&) override
  {
    using namespace textio::fmt;
    format_to<"Downloading {} ">(stdout, name);
    return true;
  }

  bool update_sizes (std::size_t total_size, std::size_t) override
  {
    using namespace textio::fmt;
    format_to<"({} bytes) ">(stdout, total_size);
    return true;
  }

  bool progress (std::span<const std::byte>, std::size_t, std::size_t) override
  {
    using namespace textio::fmt;
    format_to<".">(stdout);
    return true;
  }

  bool finalize (std::size_t) override
  {
    using namespace textio::fmt;
    format_to<"done.\n">(stdout);
    return true;
  }
};

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
auto main ([[maybe_unused]] PXENVplus& _PXENVplus, 
           [[maybe_unused]] bangPXE& _bangPXE) 
  -> void
{    
  using namespace textio::fmt;
  initialize(true);

  print_progress progress;

  {
    auto [success, buffer] = pxenv::tftp::download("netboot32.run", progress);
  }

  {
    auto [success, buffer] = pxenv::tftp::download("netboot32.sys", progress);  
  }

  asm("int $0x3");
  for(;;) 
  { 
    x86arch::yield(); 
  }

  pxenv::finalize(false);
  finalize(true);
}
