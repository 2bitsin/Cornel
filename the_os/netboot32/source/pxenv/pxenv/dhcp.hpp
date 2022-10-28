#pragma once

#include <span>
#include <tuple>

#include <textio/format.hpp>

#include <pxenv/tftp.hpp>
#include <pxenv/params.hpp>

namespace pxenv::dhcp
{

  enum class packet_type_id: std::uint16_t
  {
    dhcp_discover = 1,
    dhcp_ack      = 2,
    cached_reply  = 3
  };

  struct layout;

  struct ip_address_v4
  {
    ip_address_v4(std::uint32_t const& value): m_value(value) {};

    template <std::output_iterator<char> O>
    inline auto format(O out) const -> O 
    {
      using textio::fmt::format_to;
      return format_to<"{:d}.{:d}.{:d}.{:d}">(out, 
        ((m_value >>  0)&0xff),
        ((m_value >>  8)&0xff), 
        ((m_value >> 16)&0xff), 
        ((m_value >> 24)&0xff)
       );
    }

    auto value () const noexcept -> std::uint32_t { return m_value; }

  private:
    std::uint32_t const& m_value;    
  };

  struct client_address
  {
    client_address(std::span<const std::uint8_t> const& value): m_value(value) {};

    template <std::output_iterator<char> O>
    inline auto format(O out) const -> O 
    {
      using textio::fmt::format_to;

      out = format_to<"{:02x}">(out, m_value[0]);
      for(auto&& sub_v : m_value.subspan(1))
        out = format_to<":{:02x}">(out, sub_v);
      return out;
    }

    auto value () const noexcept -> std::span<const std::uint8_t> { return m_value; }

  private:
    std::span<const std::uint8_t> m_value;
  };

  struct info
  {
    auto        data() const -> layout const*;
    auto        size() const -> std::size_t;
    
    auto    is_valid() const noexcept -> bool;
 
    auto   client_ip() const noexcept -> ip_address_v4;
    auto     your_ip() const noexcept -> ip_address_v4;
    auto   server_ip() const noexcept -> ip_address_v4;
    auto  gateway_ip() const noexcept -> ip_address_v4;

    auto client_addr() const noexcept -> client_address;

    auto tftp_server() const noexcept -> pxenv::tftp::params;

    static auto cached (packet_type_id packet_type = packet_type_id::cached_reply) -> std::tuple<pxenv_status, dhcp::info>;

  protected:
    info(layout const* data, std::size_t size) noexcept;
    info() noexcept;

  private:
    std::size_t m_size { 0 };
    layout const* m_data { nullptr }; 

    static std::tuple<layout const*, std::size_t, std::size_t> s_cached_info [3u];
  };

}