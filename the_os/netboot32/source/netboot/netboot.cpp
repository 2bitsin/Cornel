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
#include <utils/utils.hpp>

declare_module(NETBOOT32);
static inline constexpr const std::string_view G_module_name {"NETBOOT32"}; 

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
  Gmod.info<"Starting {} ...">(G_module_name);

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

  if (!execute(G_autoexec)) 
  {
    std::abort();   
  }
}

auto Netboot::fetch(vfsio::error& error_v, std::string_view path_v, std::int32_t retry_v) -> std::span<std::byte const>
{
  error_v = vfsio::error::none;
  vfsio::archive_view archive_view_v { m_heapfile->view() };  
  if (error_v != vfsio::error::none)
    return {}; 
  auto const file_view_v = archive_view_v.find(error_v, archive_view_v.file, path_v);
  if (error_v != vfsio::error::none) 
  {
    if (retry_v > 0)
      return {};    
    if (!Netboot::download(path_v)) {
      error_v = vfsio::error::not_found;
      return {};
    }
    return fetch(error_v, path_v, retry_v + 1);
  }  
  return file_view_v;
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

auto Netboot::execute(std::string_view script_path_v) -> bool
{
  vfsio::error error_v { vfsio::error::none };
  auto const script_bytes_v = fetch(error_v, script_path_v);
  if (error_v != vfsio::error::none) {
    Gmod.error<"Failed to fetch {}: {}"> (script_path_v, (int)error_v);
    return false;
  }
  std::string script_v { utils::as_chars<char>(script_bytes_v) };
  script::interpreter interpreter_v;
  m_current_script.push(std::string(script_path_v));
  interpreter_v.execute(*this, script_v);
  auto result_v = interpreter_v.last_status();
  m_current_script.pop();
  if (!result_v || *result_v != 0) {
    Gmod.error<"Failed to execute {}: {}"> (script_path_v, result_v ? *result_v : -1);    ;
    return false;
  }
  return true;
}

int Netboot::cmd_rem(std::vector<std::string_view> const& path_v)
{
  return 0;
}

int Netboot::cmd_echo(std::vector<std::string> const& what_v) 
{   
  if (!m_current_script.empty()) {
    Glog.info<"({}) ", "">(m_current_script.top());
  }
  std::for_each(what_v.begin(), what_v.begin() + what_v.size() - 1u,   
    [](auto const& value_v) { 
      Glog.info<"{} ", "">(value_v); 
    });
  Glog.info<"{}">(what_v.back());
  return 0;
}

int Netboot::cmd_disp(std::vector<std::string_view> const& paths_v)
{
  vfsio::error error_v { vfsio::error::none };
  for (auto&& curr_v : paths_v) {
    auto file_view_v = fetch(error_v, curr_v);
    if (error_v != vfsio::error::none) {
      Gmod.error<"Failed to fetch {}: {}"> (curr_v, (int)error_v);
      return -1;
    }
    textio::fmt::format_to<"{}">(stdout, utils::as_chars<char>(file_view_v));
  }
  textio::fmt::format_to<"\n">(stdout);
  return 0;
}

int Netboot::cmd_exec(std::vector<std::string_view> const& paths_v)
{ 
  for (auto&& curr_v : paths_v) {
    if (!execute(curr_v))
      return -1;
  }
  return 0;
}

int Netboot::cmd_load(std::vector<std::string_view> const& path_v)
{
  for(auto&& curr_v : path_v) {
    if (!download(curr_v))
      return -1;
  }
  return 0;
}

int Netboot::cmd_start(std::string_view path_v) 
{
  vfsio::error error_v { vfsio::error::none };
  auto const file_view_v = fetch(error_v, path_v);
  if (error_v != vfsio::error::none) 
    goto L_error;
  auto const entry_point_v = load_image(error_v, file_view_v);
  if (error_v != vfsio::error::none) 
    goto L_error;
  entry_point_v();
L_error:
  Gmod.error<"Can't start {} (error = {:d})!"> (path_v, (int)error_v);
  return (int)error_v;
}

auto Netboot::notify_write(std::string_view path_v, vfsio::error const& error_v, std::size_t bytes_written_v) -> void
{
  auto const context_v = m_current_script.empty() ? G_module_name : m_current_script.top();
  Glog.info<"({}) Downloading {} ({} bytes) ...", "\r"> (context_v, path_v, bytes_written_v);
}

auto Netboot::notify_flush(std::string_view path_v, vfsio::error const& error_v, bool flush_succeeded_v) -> void
{
  auto const context_v = m_current_script.empty() ? G_module_name : m_current_script.top();
  Glog.info<"({}) Completeded downloading {}."> (context_v, path_v);
}

auto Netboot::notify_resize(std::string_view path_v, vfsio::error const& error_v, std::size_t size_v) -> void
{}