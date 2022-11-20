#include <pxenv/core.hpp>
#include <pxenv/tftp.hpp>
#include <pxenv/dhcp.hpp>
#include <pxenv/params.hpp>

#include <netboot/memory.hpp>

auto pxenv::tftp::get_fsize(std::string_view file_name, std::uint32_t& o_file_size, tftp::params const& options) -> ::pxenv::pxenv_status
{
  alignas(16) pxenv_tftp_get_fsize_type params;

  params.status     = pxenv_status::bad_func;
  params.server_ip  = options.server_ip;
  params.gateway_ip = options.gateway_ip;

  std::ranges::fill(params.file_name, 0u);
  if (file_name.size() > std::size(params.file_name))
    file_name = file_name.substr(0, std::size(params.file_name));
  std::ranges::copy(file_name, params.file_name);

  const auto return_status = ::pxenv::call(&params, 0x25u);
  if (!!return_status && !!params.status) {
    o_file_size = params.file_size;
  }

  return params.status;
}

auto pxenv::tftp::get_fsize(std::string_view file_name, std::uint32_t& o_file_size) -> ::pxenv::pxenv_status
{
  auto [status, dhcp_info] = dhcp::info::cached(dhcp::packet_type_id::cached_reply);
  if (status != pxenv_status::success)
    return status;
  return ::pxenv::tftp::get_fsize(file_name, o_file_size, dhcp_info.tftp_server());
}

auto pxenv::tftp::open (std::string_view file_name, std::uint16_t& packet_size, tftp::params const& options) -> ::pxenv::pxenv_status
{
  alignas(16) pxenv_tftp_open_type params;  
  params.status      = pxenv_status::bad_func;
  params.server_ip   = options.server_ip;
  params.server_port = options.port;
  params.gateway_ip  = options.gateway_ip;
  params.packet_size = packet_size;

  std::ranges::fill(params.file_name, 0u);
  if (file_name.size() > std::size(params.file_name))
    file_name = file_name.substr(0, std::size(params.file_name));
  std::ranges::copy(file_name, params.file_name);  
  const auto status = ::pxenv::call(&params, 0x20u);
  if (status == pxenv_status::success && params.status == pxenv_status::success) 
    packet_size = params.packet_size;  
  return params.status;
}

auto pxenv::tftp::open (std::string_view file_name, std::uint16_t& packet_size) -> ::pxenv::pxenv_status
{
  auto [status, dhcp_info] = dhcp::info::cached(dhcp::packet_type_id::cached_reply);
  if (status != pxenv_status::success)
    return status;  
  return pxenv::tftp::open(file_name, packet_size, dhcp_info.tftp_server());
}

auto pxenv::tftp::close () -> pxenv_status
{
  alignas(16) pxenv_tftp_close_type params;
  params.status = pxenv_status::bad_func;
  ::pxenv::call(&params, 0x21u);
  return params.status;
}

auto pxenv::tftp::read (std::span<std::byte>& o_buffer, std::uint16_t& o_packet_number) -> ::pxenv::pxenv_status
{
  alignas(16) pxenv_tftp_read_type params;
  params.status = pxenv_status::bad_func;
  params.buffer = x86arch::address16::from(o_buffer);
  params.buffer_size = std::clamp(o_buffer.size(), 0u, 0xFFFFu);
  const auto return_status = ::pxenv::call(&params, 0x22u);
  if (!!return_status && !!params.status) {
    o_packet_number = params.packet_number;
    o_buffer = o_buffer.subspan(0, params.buffer_size);
  }
  return params.status;
}

auto pxenv::tftp::download (vfsio::IFile& write_file_v, std::string_view file_name) 
  -> ::pxenv::pxenv_status
{
  auto [status, dhcp_info] = dhcp::info::cached(dhcp::packet_type_id::cached_reply);
  if (status != pxenv_status::success)
    return status;
  return pxenv::tftp::download(write_file_v, file_name, dhcp_info.tftp_server());
}

auto pxenv::tftp::download (vfsio::IFile& write_file_v, std::string_view file_name, pxenv::tftp::params options) 
  -> ::pxenv::pxenv_status
{
  pxenv_status  status      { pxenv_status::invalid_status };
  std::uint32_t bytes_read  { 0u };
  std::uint16_t last_packet { 0u };
  std::uint16_t curr_packet { 1u };
  std::uint32_t file_size   { 0u };  
  std::uint16_t packet_size { 4096u };

  memory::buffer<std::byte> packet_buffer {};

  status = pxenv::tftp::get_fsize(file_name, file_size, options);
  if (status != pxenv_status::success)
    return status;

  vfsio::error error_v { vfsio::error::none };

  status = pxenv::tftp::open(file_name, packet_size, options);
  if (status != pxenv_status::success)
    return status;  

  write_file_v.resize(error_v, file_size);
  if (error_v != vfsio::error::none)
    return pxenv_status::not_enough_memory;

  packet_buffer.allocate(memory::get_base_heap(), packet_size);  

  for (bytes_read = 0u; bytes_read < file_size; ) 
  {
    std::span<std::byte> out_packet_buffer = packet_buffer;  

    status = pxenv::tftp::read(out_packet_buffer, curr_packet);    
    if (status != pxenv_status::success)
      return status;
    if (curr_packet != last_packet + 1u) 
      return pxenv_status::tftp_invalid_packet_number;

    auto unwritten_bytes_v = write_file_v.write(error_v, out_packet_buffer);
    if (unwritten_bytes_v.empty() || error_v != vfsio::error::none)
      return pxenv_status::write_failed;    

    last_packet = curr_packet;
    bytes_read += out_packet_buffer.size();
    if (out_packet_buffer.size () < packet_size)
      break;
  }

  return pxenv::tftp::close();
}