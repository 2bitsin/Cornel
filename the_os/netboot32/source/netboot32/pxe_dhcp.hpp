#pragma once

#include <span>
#include <textio/simple.hpp>
#include <textio/simple_fmt.hpp>

#include <netboot32/pxe_interface.hpp>

namespace pxe_interface::dhcp
{
  struct dhcp_layout;

  struct ip_address_v4
  {
    ip_address_v4(std::uint32_t const& value): m_value(value) {};

    template <std::output_iterator<char> O>
    auto write(O out) -> O 
    {
      return textio::simple::write(out, (m_value >> 0)&0xff, '.', (m_value >> 8)&0xff, '.', (m_value >> 16)&0xff, '.', (m_value >> 24)&0xff);
    }

    auto value () const noexcept -> std::uint32_t { return m_value; }

  private:
    std::uint32_t const& m_value;    
  };

  struct client_address
  {
    client_address(std::span<const std::uint8_t> const& value): m_value(value) {};

    template <std::output_iterator<char> O>
    auto write(O out) -> O 
    {
      using textio::simple::write;
      using textio::simple::fmt::hex;

      out = write(out, m_value[0]);
      for(auto&& sub_v : m_value.subspan(1))
        out = write(out, ':', hex<'&'>(sub_v));      
      return out;
    }

    auto value () const noexcept -> std::span<const std::uint8_t> { return m_value; }

  private:
    std::span<const std::uint8_t> m_value;
  };

  struct packet
  {
    packet(std::span<const std::byte> buffer) noexcept;
    packet() noexcept;

    auto    is_valid() const noexcept -> bool;
 
    auto   client_ip() const noexcept -> ip_address_v4;
    auto     your_ip() const noexcept -> ip_address_v4;
    auto   server_ip() const noexcept -> ip_address_v4;
    auto  gateway_ip() const noexcept -> ip_address_v4;

    auto client_addr() const noexcept -> client_address;

    auto tftp_server() const noexcept -> pxe_interface::tftp_params;
    
  private:
     dhcp_layout const* m_layout; 
  };
}