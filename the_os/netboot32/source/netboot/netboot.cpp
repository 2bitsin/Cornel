#include <algorithm>

#include <netboot/wpnotify.hpp>
#include <netboot/netboot.hpp>
#include <netboot/memory.hpp>

#include <textio/logger.hpp>
#include <textio/format/helpers/repeat_value.hpp>

#include <memory/buffer.hpp>

#include <pxenv/core.hpp>
#include <pxenv/tftp.hpp>

#include <vfsio/vfsio.hpp>

declare_module(NETBOOT32);

using netboot::Netboot;

void Netboot::initialize()
{
  pxenv::initialize(true);
}

void Netboot::finalize()
{
  pxenv::finalize(true);
}

void Netboot::main()
{
  Gmod.info<"Starting NETBOOT32 ...">();

  vfsio::error error_v { vfsio::error::none };
  m_heapfile = std::make_unique<vfsio::heapfile>(error_v, memory::get_extended_heap_blist());
  if (error_v != vfsio::error::none) {
    Gmod.fatal<"Failed to initialize {}: {}"> ("ramdisk", (int)error_v);
    std::abort();
  }
  m_archive = std::make_unique<vfsio::archive>(error_v, m_heapfile.get());
  if (error_v != vfsio::error::none) {
    Gmod.fatal<"Failed to initialize {}: {}"> ("ramfs", (int)error_v);
    std::abort();
  }

  const auto success_v = download("netboot32.run");
  if (!success_v) {
    Gmod.fatal<"Failed to download netboot32.run">();
    std::abort();
  }
  /*
  vfsio::error error_v { vfsio::error::none };
  const auto buffer_v = m_archive->load(error_v, "netboot32.run");
  if (error_v != vfsio::error::none) {
    Gmod.fatal<"Failed to load netboot32.run: {}">((int)error_v);
    std::abort();
  }

  if (auto const result_v = execute(); !result_v || *result_v) {
    Gmod.fatal<"Failed to execute netboot32.run">();
    std::abort();
  }
  */
}

auto Netboot::download(std::string_view path_v) -> bool
{
  using namespace pxenv;
  vfsio::error error_v { vfsio::error::none };
  m_archive->open(error_v, m_archive->file, path_v);
  WPNotify<Netboot, std::string_view> outfile_v (*m_archive, *this, path_v); 
  const auto status_v = tftp::download(outfile_v, path_v);
  if (status_v != ::pxenv::pxenv_status::success) {
    Gmod.error<"Failed to download {}: {}"> (path_v, (int)status_v);
    return false;
  }
  m_archive->close(error_v);
  return true;  
}

int Netboot::cmd_echo(std::vector<std::string> const& what_v) 
{      
  std::for_each(what_v.begin(), what_v.begin() + what_v.size() - 1u, 
  [](auto const& value_v) {
    Gmod.info<"{} ", "">(value_v);
  });
  Gmod.info<"{}">(what_v.back());
  return 0;
}

int Netboot::cmd_fetch([[maybe_unused]] std::string_view designator_v, std::string_view path_v)
{ 
  const auto success_v = download(path_v);
  if (!success_v) {    
    return -1;
  }  
  return 0;
}

auto Netboot::execute(std::string_view script_v) -> std::optional<int>
{
  // TODO: figure out what's the best way to indicate failure of script execution
  script::interpreter interpreter_v;
  interpreter_v.execute(*this, script_v);
  return interpreter_v.last_status();
}


