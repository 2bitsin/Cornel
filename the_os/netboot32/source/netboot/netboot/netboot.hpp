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

  #define DEFINE_CMD(_Name_, ...) \
    struct _Name_: script::command_base<_Name_, #_Name_, __VA_ARGS__> { \
      auto execute(auto&& host_v, auto&& ... args_v) const -> int { \
        return host_v.cmd_##_Name_ (std::forward<decltype(args_v)>(args_v)...); \
      } \
    }

    DEFINE_CMD(echo, std::vector<std::string>);
    DEFINE_CMD(disp, std::vector<std::string_view>);
    DEFINE_CMD(exec, std::vector<std::string_view>);
  }

  struct Netboot: 
    public script::executor<Netboot, cmd::echo, cmd::disp, cmd::exec>
  {
    friend cmd::echo; friend cmd::disp; friend cmd::exec;

    static const inline constexpr std::string_view G_autoexec { "autoexec.bat" };

    void initialize();
    void finalize();
    void main();

  protected:
    friend WPNotify<Netboot, std::string_view>;
 
    auto fetch (vfsio::error& error_v, std::string_view path_v, std::int32_t retry_v = 0) -> std::span<std::byte const>;
    auto execute (std::string_view path_v) -> bool;
    auto download (std::string_view path_v) -> bool; 

    auto cmd_echo (std::vector<std::string> const& what_v) -> int;
    auto cmd_disp (std::vector<std::string_view> const& paths_v) -> int;
    auto cmd_exec (std::vector<std::string_view> const& paths_v) -> int;

    void notify_resize(std::string_view path_v, vfsio::error const& error_v, std::size_t size_v);
    void notify_write(std::string_view path_v, vfsio::error const& error_v, std::size_t bytes_written_v);
    void notify_flush(std::string_view path_v, vfsio::error const& error_v, bool flush_succeeded_v);

  private:
    std::unique_ptr<vfsio::heapfile> m_heapfile;
    std::unique_ptr<vfsio::archive> m_archive;
    std::stack<std::string> m_current_script;
    
  };
}