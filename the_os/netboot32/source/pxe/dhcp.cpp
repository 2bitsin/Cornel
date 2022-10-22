#include <pxe/dhcp.hpp>

namespace pxe::dhcp
{
  #pragma pack(push, 1)
  struct dhcp_layout
  {
    std::uint8_t  op;
    std::uint8_t  htype;
    std::uint8_t  hlen;
    std::uint8_t  hops;

    std::uint32_t xid;

    std::uint16_t secs;
    std::uint16_t flags;

    std::uint32_t ciaddr;
    std::uint32_t yiaddr;
    std::uint32_t siaddr;
    std::uint32_t giaddr;

    std::uint8_t  chaddr[16];
    std::uint8_t  sname[64];
    std::uint8_t  file[128];
    std::uint32_t magic_cookie;    
  };
  #pragma pack(pop)



  packet::packet() noexcept: m_layout{nullptr}  
  {}

  packet::packet(std::span<const std::byte> buffer) noexcept: packet()
  {    
    if (buffer.size() >= sizeof(dhcp_layout)) {
      m_layout = (dhcp_layout const*)buffer.data();
    }    
  }

  auto packet::is_valid() const noexcept -> bool
  {
    return m_layout;
  }

  
  auto packet::client_ip() const noexcept -> ip_address_v4
  {
    return m_layout->ciaddr;
  }

  auto packet::your_ip() const noexcept -> ip_address_v4
  {
    return m_layout->yiaddr;
  }

  auto packet::server_ip() const noexcept -> ip_address_v4
  {
    return m_layout->siaddr;
  }

  auto packet::gateway_ip() const noexcept -> ip_address_v4
  {
    return m_layout->giaddr;
  }

  auto packet::client_addr() const noexcept -> client_address
  {
    return client_address({&m_layout->chaddr[0], m_layout->hlen});
  }

  auto packet::tftp_server() const noexcept -> ::pxe::tftp_params
  {
    return ::pxe::tftp_params {
      .server_ip  = server_ip().value(),
      .port       = 0x4500u,
      .gateway_ip = gateway_ip().value()
    };
  }

}