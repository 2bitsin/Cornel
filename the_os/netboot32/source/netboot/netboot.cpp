#include <algorithm>

#include <netboot/download_notify.hpp>
#include <netboot/netboot.hpp>

#include <textio/logger.hpp>
#include <textio/format/helpers/repeat_value.hpp>

#include <memory/buffer.hpp>

#include <pxenv/core.hpp>
#include <pxenv/tftp.hpp>

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

  auto const [success_v, buffer_v] = download("netboot32.run");
  if (!success_v || buffer_v.empty()) {
    Gmod.fatal<"Nothing to do.">();
    std::abort();  
  }
    
  if (auto const result_v = execute(buffer_v); !result_v || *result_v) {
    Gmod.fatal<"Failed to execute netboot32.run">();
    std::abort();
  }
}

auto Netboot::download(std::string_view path_v) -> std::tuple<bool, memory::buffer<std::byte>>
{
  using namespace pxenv;
  DownloadNotify<Netboot> notify_v (*this);
  auto[status_v, buffer_v] = tftp::download(path_v, notify_v);
  return { status_v == pxenv_status::success, std::move(buffer_v) };  
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
  auto const [success_v, buffer_v] = download(path_v);
  if (!success_v) {    
    return -1;
  }
  
  return 0;
}

bool Netboot::download_initialize ([[maybe_unused]] std::string_view& file_name_v, [[maybe_unused]] ::pxenv::tftp::params& params_v) 
{ 
  m_download_file_name = file_name_v;
  return true; 
}

bool Netboot::download_update_sizes ([[maybe_unused]] std::size_t total_size_v, [[maybe_unused]] std::size_t packet_size_v) 
{ 
  m_download_total_size = total_size_v;
  return true; 
}

bool Netboot::download_progress ([[maybe_unused]] std::span<const std::byte> buffer_s, [[maybe_unused]] std::size_t packet_number_v, [[maybe_unused]] std::size_t offset_v) 
{ 
  Gmod.info<"Downloading {} ... {}/{} bytes", "\r">(m_download_file_name, offset_v + buffer_s.size(), m_download_total_size);
  return true; 
}

bool Netboot::download_finalize ([[maybe_unused]] std::size_t total_bytes_v) 
{ 
  using textio::fmt::helpers::repeat_value;
  Gmod.info<"Downloading {} ... finished.{}">(m_download_file_name, 
    repeat_value<10, char>(' '));
  return true; 
}

auto Netboot::download_failure (::pxenv::pxenv_status status) -> ::pxenv::pxenv_status 
{ 
  Gmod.error<"Download failed with status {:04x}.">((std::uint16_t)status);
  return status; 
}


auto Netboot::execute(std::string_view script_v) -> std::optional<int>
{
  // TODO: figure out what's the best way to indicate failure of script execution
  script::interpreter interpreter_v;
  interpreter_v.execute(*this, script_v);
  return interpreter_v.last_status();
}


