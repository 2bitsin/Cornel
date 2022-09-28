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
#include <netboot32/panick.hpp>
#include <netboot32/dhcp.hpp>

#include <memory/allocate_buffer.hpp>

#include <utils/bits.hpp>
#include <utils/version.hpp>

static x86arch::real_address G_pxe_entry_point = { 0u, 0u };
static dhcp::packet G_cached_dhcp_reply;

static void initialize_ftp()
{
  using namespace textio::simple::fmt;
  using namespace pxe_interface;
  std::span<const std::byte> cached_reply_s;
  std::uint16_t limit { 0 };
  if (!get_cached_info(packet_type::cached_reply, cached_reply_s, limit)) {
    return panick::pxe_failed("failed to get cached reply packet");
  }

#ifndef NO_DEBUG_LOG
  console::writeln("Cached reply packet : ");
  console::writeln("  * cached_reply_s.size : ", cached_reply_s.size());
  console::writeln("  * cached_reply_s.data : ", hex<'&'>(cached_reply_s.data()));
#endif  

  std::construct_at(&G_cached_dhcp_reply, cached_reply_s);
  if (!G_cached_dhcp_reply.is_valid())
    return panick::pxe_failed("Cached reply packet is invalid");

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
  auto address_of_params = real_address::from_pointer(params);
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
  params.status = pxenv_status::invalid_status;
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

  params.status     = pxenv_status::invalid_status;
  params.server_ip  = options.server_ip;
  params.gateway_ip = options.gateway_ip;

  std::ranges::fill(params.file_name, 0u);
  if (file_name.size() > std::size(params.file_name))
    file_name = file_name.substr(0, std::size(params.file_name));
  std::ranges::copy(file_name, params.file_name);

  const auto return_status = pxe_interface_call(&params, 0x25u);

  if (!!return_status && !!params.status)
  {
    o_file_size = params.file_size;
  }

  return params.status;
}

auto pxe_interface::tftp_get_fsize(std::string_view file_name, std::uint32_t& o_file_size) -> pxenv_status
{
  if (!G_cached_dhcp_reply.is_valid())
    return pxenv_status::tftp_cannot_open_connection;
  pxe_interface::tftp_params options { 
    .server_ip = G_cached_dhcp_reply.server_ip().value(),     
    .port = 69u,
    .packet_size = 512u,
    .gateway_ip = G_cached_dhcp_reply.gateway_ip().value()
  };
  return pxe_interface::tftp_get_fsize(file_name, o_file_size, options);
}

