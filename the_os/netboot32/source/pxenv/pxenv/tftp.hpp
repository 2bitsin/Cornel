#pragma once

#include <pxenv/core.hpp>
#include <pxenv/tftp_notify.hpp>

#include <memory/buffer.hpp>

namespace pxenv::tftp
{
  struct params
  {
    std::uint32_t server_ip   { 0u      };
    std::uint16_t port        { 0x4500u };
    std::uint32_t gateway_ip  { 0u      };
  }; 

  
  auto get_fsize   (std::string_view file_name, std::uint32_t& o_file_size, params const& options) -> pxenv_status;
  auto get_fsize   (std::string_view file_name, std::uint32_t& o_file_size) -> pxenv_status;
  auto open        (std::string_view file_name, std::uint16_t& o_packet_size, params const& options) -> pxenv_status;
  auto open        (std::string_view file_name, std::uint16_t& o_packet_size) -> pxenv_status;
  auto read        (std::span<std::byte>& buffer, std::uint16_t& o_packet_number) -> pxenv_status;
  auto close       () -> pxenv_status;

  // --------------------------------------- utility functions --------------------------------------

  auto download    (std::string_view file_name, params options, Inotify& notify = Inotify::null()) -> std::tuple<pxenv_status, memory::buffer<std::byte>>; 
  auto download    (std::string_view file_name, Inotify& notify = Inotify::null()) -> std::tuple<pxenv_status, memory::buffer<std::byte>>; 
}