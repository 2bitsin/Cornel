#include <cstdint>
#include <cstddef>

#include <span>
#include <string_view>

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

static x86arch::real_address G_pxe_entry_point = { 0u, 0u };

static void initialize([[maybe_unused]] bangPXE& pxe_s)
{
  G_pxe_entry_point = pxe_s.entry_point_16;
}

static void initialize([[maybe_unused]] PXENVplus& pxe_s)
{
  G_pxe_entry_point = pxe_s.entry_point_16;
}

static void initialize_ftp()
{
  using namespace textio::simple::fmt;
  using namespace pxe_interface;
  std::span<const std::byte> cached_reply_s;
  std::uint16_t limit { 0 };
  if (get_cached_info(packet_type::cached_reply, cached_reply_s, limit))
    return panick::pxe_failed("failed to get cached reply packet");
  console::writeln("Cached reply packet : ");  
  console::writeln("  * buffer.data() . : ", hex<'&'>(cached_reply_s.data()));
  console::writeln("  * buffer.size() . : ", cached_reply_s.size());

  dhcp::packet cached_reply_p { cached_reply_s };
  if (!cached_reply_p.is_valid())
    return panick::pxe_failed("Cached reply packet is invalid");
  console::writeln("  * client IP ..... : ", cached_reply_p.client_ip());
  console::writeln("  * your IP ....... : ", cached_reply_p.your_ip());
  console::writeln("  * server IP ..... : ", cached_reply_p.server_ip());
  console::writeln("  * gateway IP .... : ", cached_reply_p.gateway_ip());
  console::writeln("  * client MAC .... : ", cached_reply_p.client_addr());
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

void pxe_interface::initialize(bool first_time, PXENVplus& pxenvplus_s, bangPXE& bangpxe_s)
{
  if (!first_time)
    return;

  if (!validate_PXENVplus(pxenvplus_s))
    return panick::invalid_pxenvplus();

  const auto [v_min, v_maj] = bits::unpack_as_tuple<8, 8>(pxenvplus_s.version);
  console::writeln("Initializing PXE v", v_min, ".", v_maj, " ... ");

  if (pxenvplus_s.version >= 0x201)
  {
    if (!validate_bangPXE(bangpxe_s)) {
      return panick::invalid_bangpxe();
    }

    ::initialize(bangpxe_s);
  }
  else 
  {
    ::initialize(pxenvplus_s);     
  }
  initialize_ftp();
}

void pxe_interface::finalize(bool last_time)
{
  if(!last_time)
    return;
}

auto pxe_interface::get_cached_info(std::uint16_t packet_type, std::span<const std::byte>& buffer, std::uint16_t& buffer_limit) -> std::uint16_t
{
  using namespace x86arch;
  
  alignas(16) pxenv_get_cached_info_type params;
  params.status = 0xffffu;
  params.packet_type = packet_type;
  params.buffer_limit = 0u;
  params.buffer_size = 0u;
  params.buffer = segoff{ 0, 0 };
  auto address_of_params = real_address::from_pointer(&params);

  call16_context ctx;
  call16_stack stack{ ctx, 0x1000u };
  stack.push<std::uint16_t>(address_of_params.seg);
  stack.push<std::uint16_t>(address_of_params.off);
  stack.push<std::uint16_t>(0x0071u);
  ctx.flags |= flags::interrupt;
  ctx.eax = ~0x0u;
  call16_invoke(ctx, G_pxe_entry_point);
  if (!ctx.ax && !params.status)  
  {
    buffer_limit = params.buffer_limit;
    auto buff_ptr = real_address{ params.buffer.off, params.buffer.seg }.to_pointer<std::byte>();
    buffer = std::span{ buff_ptr, params.buffer_size };    
  } 
  return params.status;
}

