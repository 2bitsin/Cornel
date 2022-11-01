#pragma once

#include <vector>
#include <string_view>
#include <string>
#include <tuple>

#include <script/interpreter.hpp>
#include <script/executor.hpp>

#include <pxenv/core.hpp>
#include <pxenv/tftp.hpp>

#include <memory/buffer.hpp>

#include <netboot/download_notify.hpp>

namespace netboot
{
  namespace cmd
  {
    // echo [what...]
    struct echo: script::command_base<echo, "echo", std::vector<std::string>>
    {
      auto execute(auto&& netboot_v, auto&& ... args_v)  
      { return netboot_v.cmd_echo(std::forward<decltype(args_v)>(args_v)...); }
    };

    // fetch <designator> <resource path> 
    struct fetch: script::command_base<fetch, "fetch", std::string_view, std::string_view>
    {
      auto execute(auto&& netboot_v, auto&& ... args_v)  
      { return netboot_v.cmd_fetch(std::forward<decltype(args_v)>(args_v)...); }
    };
  }

  struct Netboot: 
    public script::executor<Netboot, cmd::echo, cmd::fetch>
  {
    void initialize();
    void finalize();
    void main();

  protected:
    friend DownloadNotify<Netboot>;
    friend cmd::echo;
    friend cmd::fetch;
 
    auto download (std::string_view path_v) -> std::tuple<bool, memory::buffer<std::byte>>;
    auto execute (std::string_view script_v) -> std::optional<int>;
    auto register_resource (std::string_view designator_v, memory::buffer<std::byte> buffer_v) -> bool;

    auto cmd_echo (std::vector<std::string> const& what_v) -> int;
    auto cmd_fetch (std::string_view designator_v, std::string_view path_v) -> int;
  
    bool download_initialize (std::string_view& file_name_v, ::pxenv::tftp::params& params_v);
    bool download_update_sizes (std::size_t total_size_v, std::size_t packet_size_v);
    bool download_progress (std::span<const std::byte> buffer_s, std::size_t packet_number_v, std::size_t offset_v);
    bool download_finalize (std::size_t total_bytes_v);
    auto download_failure (::pxenv::pxenv_status status) -> ::pxenv::pxenv_status;

  private:
    std::string_view m_download_file_name;
    std::size_t      m_download_total_size;
  };
}