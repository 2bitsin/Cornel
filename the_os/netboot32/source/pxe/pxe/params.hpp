#pragma once

namespace pxe
{
  struct tftp_params
  {
    std::uint32_t server_ip   { 0u      };
    std::uint16_t port        { 0x4500u };
    std::uint32_t gateway_ip  { 0u      };
  }; 
}