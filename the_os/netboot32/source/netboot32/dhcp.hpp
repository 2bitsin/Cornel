#pragma once

#include <span>
#include <textio/simple.hpp>
#include <textio/simple_fmt.hpp>

namespace dhcp
{
  struct dhcp_layout;

  struct ip_address_v4
  {
    ip_address_v4(std::uint32_t const& value): value(value) {};

    template <std::output_iterator<char> O>
    auto write(O out) -> O 
    {
      return textio::simple::write(out, (value >> 0)&0xff, '.', (value >> 8)&0xff, '.', (value >> 16)&0xff, '.', (value >> 24)&0xff);
    }

    std::uint32_t const& value;    
  };

  struct client_address
  {
    client_address(std::span<const std::uint8_t> const& value): value(value) {};

    template <std::output_iterator<char> O>
    auto write(O out) -> O 
    {
      using textio::simple::write;
      using textio::simple::fmt::hex;

      out = write(out, value[0]);
      for(auto&& sub_v : value.subspan(1))
        out = write(out, ':', hex<'&'>(sub_v));      
      return out;
    }

    std::span<const std::uint8_t> value;
  };

  struct packet
  {
    packet(std::span<const std::byte> buffer) noexcept;

    auto    is_valid() const noexcept -> bool;
 
    auto   client_ip() const noexcept -> ip_address_v4;
    auto     your_ip() const noexcept -> ip_address_v4;
    auto   server_ip() const noexcept -> ip_address_v4;
    auto  gateway_ip() const noexcept -> ip_address_v4;

    auto client_addr() const noexcept -> client_address;
    
  private:
     dhcp_layout const* m_layout; 
  };
}