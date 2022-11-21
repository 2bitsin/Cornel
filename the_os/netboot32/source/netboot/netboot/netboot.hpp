#pragma once

#include <vector>
#include <string_view>
#include <string>
#include <tuple>
#include <memory>

#include <netboot/wpnotify.hpp>

#include <script/interpreter.hpp>
#include <script/executor.hpp>

#include <pxenv/core.hpp>
#include <pxenv/tftp.hpp>

#include <memory/buffer.hpp>

#include <vfsio/archive.hpp>
#include <vfsio/archive_view.hpp>
#include <vfsio/heapfile.hpp>

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
    friend WPNotify<Netboot, std::string_view>;
    friend cmd::echo;
    friend cmd::fetch;
 
    auto download (std::string_view path_v) -> bool;
    auto execute (std::string_view path_v) ->bool;
    auto download_and_execute (std::string_view path_v) -> bool;

    auto cmd_echo (std::vector<std::string> const& what_v) -> int;
    auto cmd_fetch (std::string_view designator_v, std::string_view path_v) -> int;

    void notify_resize(std::string_view path_v, vfsio::error const& error_v, std::size_t size_v);
    void notify_write(std::string_view path_v, vfsio::error const& error_v, std::size_t bytes_written_v);

  private:
    std::unique_ptr<vfsio::heapfile> m_heapfile;
    std::unique_ptr<vfsio::archive> m_archive;
  };
}