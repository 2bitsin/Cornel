#include <pxenv/core.hpp>
#include <pxenv/tftp.hpp>
#include <pxenv/dhcp.hpp>
#include <pxenv/params.hpp>

namespace pxenv::dhcp
{
  static auto get_cached_info(packet_type_id pktid, auto& data_o, auto& size_o, auto& limit_o)
    -> pxenv_status
  {
    alignas(16) pxenv_get_cached_info_type params; 
    params.status = pxenv_status::bad_func;
    params.packet_type = (std::uint16_t)pktid;
    params.buffer_limit = 0u;
    params.buffer_size = 0u;
    params.buffer = segoff{ 0, 0 };
    const auto return_status = ::pxenv::call(&params, 0x71);
    if (return_status != pxenv_status::success 
      ||params.status != pxenv_status::success)
      return params.status;
    data_o = x86arch::address16{ params.buffer.off, params.buffer.seg }.as<dhcp::layout const*>();
    limit_o = params.buffer_limit;
    size_o = params.buffer_size;
    return params.status;
  }

#pragma pack(push, 1)
  struct layout
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

  info::info() noexcept
  : info(nullptr, 0u)
  {}

  info::info(layout const* data, std::size_t size) noexcept
  : m_size { size }
  , m_data { data }     
  {}

  auto info::data() const -> layout const*
   { return m_data; }

  auto info::size() const -> std::size_t
  { return m_size; }

  auto info::is_valid() const noexcept -> bool
  {
    return sizeof(layout) <= m_size && nullptr != m_data;
  }
  
  auto info::client_ip() const noexcept -> ip_address_v4
  {
    return m_data->ciaddr;
  }

  auto info::your_ip() const noexcept -> ip_address_v4
  {
    return m_data->yiaddr;
  }

  auto info::server_ip() const noexcept -> ip_address_v4
  {
    return m_data->siaddr;
  }

  auto info::gateway_ip() const noexcept -> ip_address_v4
  {
    return m_data->giaddr;
  }

  auto info::client_addr() const noexcept -> client_address
  {
    return client_address({&m_data->chaddr[0], m_data->hlen});
  }

  auto info::tftp_server() const noexcept -> ::pxenv::tftp::params
  {
    return ::pxenv::tftp::params 
    {
      .server_ip  = server_ip().value(),
      .port       = 0x4500u,
      .gateway_ip = gateway_ip().value()
    };
  }

  auto info::cached(packet_type_id pktid) -> std::tuple<pxenv_status, dhcp::info>
  {
    auto [data, size, limit] = info::s_cached_info [((int)pktid - 1)];
    if (nullptr == data || size < sizeof(layout))      
    {
      if (auto status = get_cached_info(pktid, data, size, limit); pxenv_status::success != status)
        return { status, info {} };
      info::s_cached_info [((int)pktid - 1)] = std::tuple{ data, size, limit };      
    }    
    return { pxenv_status::success, dhcp::info{ data, size } };
  }

  std::tuple<dhcp::layout const*, std::size_t, std::size_t> info::s_cached_info [3];

}

