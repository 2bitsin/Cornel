#include <cstdint>
#include <cstddef>
#include <span>
#include <string_view>
#include <utility>
#include <algorithm>
#include <ranges>

#include <hardware/console.hpp>
#include <hardware/x86flags.hpp>
#include <hardware/x86real_addr.hpp>
#include <hardware/x86call16.hpp>
#include <hardware/x86call16_stack.hpp>

#include <netboot32/pxe_interface.hpp>
#include <netboot32/pxe_structs.hpp>
#include <netboot32/pxe_dhcp.hpp>
#include <netboot32/panick.hpp>
#include <netboot32/memory.hpp>

#include <memory/buffer.hpp>

#include <utils/bits.hpp>
#include <utils/int.hpp>
#include <utils/version.hpp>
#include <utils/defer.hpp>

static x86arch::real_address G_pxe_entry_point = { 0u, 0u };
static pxe_interface::dhcp::packet G_cached_dhcp_reply;

static void initialize_ftp()
{
  using namespace textio::simple::fmt;
  using namespace pxe_interface;
  std::span<const std::byte> cached_reply_s;
  std::uint16_t limit { 0 };
  if (!get_cached_info(packet_type::cached_reply, cached_reply_s, limit)) 
  { return panick::pxe_failed("failed to get cached reply packet"); }
  
#ifndef NO_DEBUG_LOG
  console::writeln("Cached reply packet : ");
  console::writeln("  * cached_reply_s.size : ", cached_reply_s.size());
  console::writeln("  * cached_reply_s.data : ", hex<'&'>(cached_reply_s.data()));
#endif  

  std::construct_at(&G_cached_dhcp_reply, cached_reply_s);
  if (!G_cached_dhcp_reply.is_valid())
  { return panick::pxe_failed("Cached reply packet is invalid"); }

#ifndef NO_DEBUG_LOG
  console::writeln("  * client IP ..... : ", G_cached_dhcp_reply.client_ip());
  console::writeln("  * your IP ....... : ", G_cached_dhcp_reply.your_ip());
  console::writeln("  * server IP ..... : ", G_cached_dhcp_reply.server_ip());
  console::writeln("  * gateway IP .... : ", G_cached_dhcp_reply.gateway_ip());
  console::writeln("  * client MAC .... : ", G_cached_dhcp_reply.client_addr());
#endif 
}

static auto validate_bangPXE(bangPXE const& pxe_s) -> bool
{
  using namespace std::string_view_literals;
  std::string_view signature_view(pxe_s.signature, std::size(pxe_s.signature));
  if (signature_view != "!PXE"sv) 
    return false;
  std::span byte_span_s{ (std::uint8_t const*)&pxe_s, pxe_s.length };
  std::uint8_t byte_based_sum{ 0u };
  for (auto&& single_byte : byte_span_s)
    byte_based_sum += single_byte;
  if (0u != byte_based_sum)
    return false; 
  return true;
}

static auto validate_PXENVplus(PXENVplus const& pxe_s) -> bool
{
  using namespace std::string_view_literals;
  std::string_view signature_view(pxe_s.signature, std::size(pxe_s.signature));
  if (signature_view != "PXENV+"sv) 
    return false;
  std::span byte_span_s{ (std::uint8_t const*)&pxe_s, pxe_s.length };
  std::uint8_t byte_based_sum{ 0u };
  for (auto&& single_byte : byte_span_s)
    byte_based_sum += single_byte;
  if (0u != byte_based_sum)
    return false; 
  return true;
}

static auto initialize_pxe(PXENVplus& pxenvplus_s, bangPXE& bangpxe_s) -> version<std::uint8_t, std::uint8_t>
{
  if (!validate_PXENVplus(pxenvplus_s)) {
    panick::invalid_pxenvplus();
  }

  if (pxenvplus_s.version >= 0x201u)
  {
  #ifndef NO_DEBUG_LOG
    console::writeln("Using !PXE");
  #endif
    if (!validate_bangPXE(bangpxe_s))
    {
      panick::invalid_bangpxe();
    }      
    G_pxe_entry_point = bangpxe_s.entry_point_16;
  }
  else 
  {
  #ifndef NO_DEBUG_LOG
    console::writeln("Using PXENV+");
  #endif
    G_pxe_entry_point = pxenvplus_s.entry_point_16;
  }
#ifndef NO_DEBUG_LOG
  console::writeln("PXE entry point : ", G_pxe_entry_point);
#endif
  return version<>::from_word<8, 8>(pxenvplus_s.version);
}

void pxe_interface::initialize(bool first_time, PXENVplus& pxenvplus_s, bangPXE& bangpxe_s)
{
  if (!first_time)
    return;

  const auto pxe_version = initialize_pxe(pxenvplus_s, bangpxe_s);  
  console::writeln("Initialized PXE v", pxe_version, " ... ");
  initialize_ftp();
}

void pxe_interface::finalize(bool last_time)
{
  if(!last_time)
    return;
}

auto pxe_interface_call(void* params, std::uint16_t opcode) -> pxenv_status
{
  using namespace x86arch;
  auto address_of_params = real_address::from(params);
  call16_context ctx;
  std::memset(&ctx, 0, sizeof(ctx));
  call16_stack stack{ ctx, 0x1000u };
  stack.push<std::uint16_t>(address_of_params.seg);
  stack.push<std::uint16_t>(address_of_params.off);
  stack.push<std::uint16_t>(opcode);
  ctx.irq_mask = 0u;
  ctx.flags |= flags::interrupt;
  ctx.eax = std::to_underlying (pxenv_status::invalid_status);
  ctx.ebx = opcode;
  ctx.es  = address_of_params.seg;
  ctx.ds  = address_of_params.seg;
  ctx.edi = address_of_params.off;
  call16_invoke(ctx, G_pxe_entry_point);
  return static_cast<pxenv_status>(ctx.ax);
}

auto pxe_interface::get_cached_info(std::uint16_t packet_type, std::span<const std::byte>& buffer, std::uint16_t& buffer_limit) -> pxenv_status
{
  alignas(16) pxenv_get_cached_info_type params; 
  params.status = pxenv_status::bad_func;
  params.packet_type = packet_type;
  params.buffer_limit = 0u;
  params.buffer_size = 0u;
  params.buffer = segoff{ 0, 0 };
  const auto return_status = pxe_interface_call(&params, 0x71);
  if (return_status != pxenv_status::success 
    ||params.status != pxenv_status::success)
    return params.status;
  buffer_limit = params.buffer_limit;
  auto buff_ptr = x86arch::real_address{ params.buffer.off, params.buffer.seg }.to_pointer<std::byte>();
  buffer = std::span{ buff_ptr, params.buffer_size };    
  return params.status;
}

auto pxe_interface::tftp_get_fsize(std::string_view file_name, std::uint32_t& o_file_size, tftp_params const& options) -> pxenv_status
{
  alignas(16) pxenv_tftp_get_fsize_type params;

  params.status     = pxenv_status::bad_func;
  params.server_ip  = options.server_ip;
  params.gateway_ip = options.gateway_ip;

  std::ranges::fill(params.file_name, 0u);
  if (file_name.size() > std::size(params.file_name))
    file_name = file_name.substr(0, std::size(params.file_name));
  std::ranges::copy(file_name, params.file_name);

  const auto return_status = pxe_interface_call(&params, 0x25u);
  if (!!return_status && !!params.status) {
    o_file_size = params.file_size;
  }

  return params.status;
}

auto pxe_interface::tftp_get_fsize(std::string_view file_name, std::uint32_t& o_file_size) -> pxenv_status
{
  if (!G_cached_dhcp_reply.is_valid()) 
  { return pxenv_status::invalid_cached_dhcp_replay; }
  return pxe_interface::tftp_get_fsize(file_name, o_file_size, G_cached_dhcp_reply.tftp_server());
}

auto pxe_interface::tftp_open (std::string_view file_name, std::uint16_t& packet_size, tftp_params const& options) -> pxenv_status
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
  const auto status = pxe_interface_call(&params, 0x20u);
  if (status == pxenv_status::success && params.status == pxenv_status::success) 
    packet_size = params.packet_size;  
  return params.status;
}

auto pxe_interface::tftp_open (std::string_view file_name, std::uint16_t& packet_size) -> pxenv_status
{
  if (!G_cached_dhcp_reply.is_valid())
  { return pxenv_status::invalid_cached_dhcp_replay; }
  
  return pxe_interface::tftp_open(file_name, packet_size, G_cached_dhcp_reply.tftp_server());
}

auto pxe_interface::tftp_close () -> pxenv_status
{
  alignas(16) pxenv_tftp_close_type params;
  params.status = pxenv_status::bad_func;
  pxe_interface_call(&params, 0x21u);
  return params.status;
}

auto pxe_interface::tftp_read (std::span<std::byte>& o_buffer, std::uint16_t& o_packet_number) -> pxenv_status
{
  alignas(16) pxenv_tftp_read_type params;
  params.status = pxenv_status::bad_func;
  params.buffer = x86arch::real_address::from(o_buffer);
  params.buffer_size = std::clamp(o_buffer.size(), 0u, 0xFFFFu);
  const auto return_status = pxe_interface_call(&params, 0x22u);
  if (!!return_status && !!params.status) {
    o_packet_number = params.packet_number;
    o_buffer = o_buffer.subspan(0, params.buffer_size);
  }
  return params.status;
}

auto pxe_interface::download_file (std::string_view file_name, ::memory::buffer<std::byte>& o_buffer) -> pxenv_status
{
  if (!G_cached_dhcp_reply.is_valid()) 
  { return pxenv_status::invalid_cached_dhcp_replay; }  
  return pxe_interface::download_file(file_name, o_buffer, G_cached_dhcp_reply.tftp_server());
}

auto pxe_interface::download_file (std::string_view file_name, ::memory::buffer<std::byte>& o_buffer, pxe_interface::tftp_params const& options) -> pxenv_status
{
  using namespace textio::simple::fmt;

  pxenv_status  status      { pxenv_status::invalid_status };
  std::uint32_t bytes_read  { 0u };
  std::uint16_t last_packet { 0u };
  std::uint16_t curr_packet { 1u };
  std::uint32_t file_size   { 0u };  
  std::uint16_t packet_size { 4096u };

  memory::buffer<std::byte> entire_buffer {};
  memory::buffer<std::byte> packet_buffer {};

  status = pxe_interface::tftp_get_fsize(file_name, file_size, options);
  if (status != pxenv_status::success)
    return status;
  status = pxe_interface::tftp_open(file_name, packet_size, options);
  if (status != pxenv_status::success)
    return status;

  entire_buffer.allocate(memory::get_extended_heap(), file_size);    
  packet_buffer.allocate(memory::get_base_heap(), packet_size);  

  for (bytes_read = 0u; bytes_read < file_size; ) 
  {
    std::span<std::byte> out_packet_buffer = packet_buffer;  
    status = pxe_interface::tftp_read(out_packet_buffer, curr_packet);    
    if (status != pxenv_status::success)
      return status;
    if (curr_packet != last_packet + 1u) 
      return pxenv_status::tftp_invalid_packet_number;
    auto dest_buffer_range = entire_buffer.subspan(bytes_read, out_packet_buffer.size());
    std::ranges::copy(out_packet_buffer, std::begin(dest_buffer_range));
    last_packet = curr_packet;
    bytes_read += out_packet_buffer.size();
    if (out_packet_buffer.size () < packet_size)
      break;    
  }
  pxe_interface::tftp_close();
  o_buffer = std::move(entire_buffer);  
  return status;
}