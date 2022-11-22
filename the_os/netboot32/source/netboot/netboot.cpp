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

  if (!download_and_execute("netboot32.run"))
    std::abort();   
}

auto Netboot::download_and_execute(std::string_view path_v) -> bool
{
  return download(path_v) && execute(path_v);
}

auto Netboot::download(std::string_view path_v) -> bool
{
  using namespace pxenv;
  vfsio::error error_v { vfsio::error::none };
  m_archive->open(error_v, m_archive->file, path_v);
  WPNotify<Netboot, std::string_view> outfile_v (*m_archive, *this, path_v); 
  const auto status_v = tftp::download(outfile_v, path_v);
  if (status_v != ::pxenv::pxenv_status::success) {
    Gmod.error<"Failed to download {}: {:04x}"> (path_v, (unsigned)status_v);
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
  return download(path_v) ? 0 : -1;
}

auto Netboot::execute(std::string_view script_path_v) -> bool
{
  // TODO: figure out what's the best way to indicate failure of script execution
  vfsio::archive_view archive_view_v { m_heapfile->view() };  
  vfsio::error error_v { vfsio::error::none };
  auto script_bytes_v = archive_view_v.find(error_v, archive_view_v.file, script_path_v);
  if (error_v != vfsio::error::none) {
    Gmod.error<"Failed to find {}: {}"> (script_path_v, (int)error_v);
    return false;
  }
  std::string script_v { (char const*)script_bytes_v.data(), script_bytes_v.size() };
  script::interpreter interpreter_v;
  interpreter_v.execute(*this, script_v);
  auto result_v = interpreter_v.last_status();
  if (!result_v || *result_v != 0) {
    Gmod.error<"Failed to execute {}: {}"> (script_path_v, result_v ? *result_v : -1);    ;
    return false;
  }
  return true;
}

auto Netboot::notify_write(std::string_view path_v, vfsio::error const& error_v, std::size_t bytes_written_v) -> void
{
  Gmod.info<"Downloading {} ({} bytes) ...", "\r"> (path_v, bytes_written_v);
}

auto Netboot::notify_flush(std::string_view path_v, vfsio::error const& error_v, bool flush_succeeded_v) -> void
{
  Gmod.info<"Completeded downloading {}."> (path_v);
}

auto Netboot::notify_resize(std::string_view path_v, vfsio::error const& error_v, std::size_t size_v) -> void
{}