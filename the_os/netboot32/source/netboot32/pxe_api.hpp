#pragma once

#include <cstddef>
#include <cstdint>


struct PXENVplus;
struct bangPXE;

struct initialize_context
{
  PXENVplus& m_PXENVplus;
  bangPXE&   m_bangPXE;
};

namespace pxe_api
{
  static inline constexpr const std::uint16_t PACKET_TYPE_DHCP_DISCOVER = 1;
  static inline constexpr const std::uint16_t PACKET_TYPE_DHCP_ACK      = 2;
  static inline constexpr const std::uint16_t PACKET_TYPE_CACHED_REPLY  = 3;

  auto initialize(bool first_time, initialize_context const&) -> void;
  auto finalize(bool last_time, initialize_context const&) -> void;
  auto get_cached_info(std::uint16_t packet_type, std::span<const std::byte>& buffer, std::uint16_t& buffer_limit) -> std::uint16_t;
};
