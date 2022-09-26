#pragma once

#include <cstddef>
#include <cstdint>
#include <span>

struct PXENVplus;
struct bangPXE;

namespace pxe_interface
{
  namespace packet_type
  {
    static inline constexpr const std::uint16_t dhcp_discover = 1;
    static inline constexpr const std::uint16_t dhcp_ack      = 2;
    static inline constexpr const std::uint16_t cached_reply  = 3;
  }

  auto initialize(bool first_time, ::PXENVplus&, ::bangPXE&) -> void;
  auto finalize(bool last_time) -> void;
  auto get_cached_info(std::uint16_t packet_type, std::span<const std::byte>& buffer, std::uint16_t& buffer_limit) -> std::uint16_t;
  
};
