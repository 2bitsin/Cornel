#include <cstdint>
#include <cstddef>
#include <span>
#include <string_view>
#include <utility>
#include <algorithm>
#include <ranges>

#include <hardware/x86address16.hpp>
#include <hardware/x86call16.hpp>
#include <hardware/x86call16_stack.hpp>
#include <hardware/x86assembly.hpp>

#include <pxenv/core.hpp>
#include <pxenv/params.hpp>
#include <pxenv/dhcp.hpp>

#include <netboot/panick.hpp>
#include <netboot/memory.hpp>

#include <memory/buffer.hpp>

#include <utils/bits.hpp>
#include <utils/int.hpp>
#include <utils/version.hpp>
#include <utils/defer.hpp>

static x86arch::address16 G_pxe_entry_point = { 0u, 0u };

static auto pxe_installation_check(PXENVplus* (&pxenvplus)) -> pxenv::pxenv_status
{
  using namespace x86arch;
  using enum pxenv::pxenv_status;

  call16_context regs;
  call16_stack stack { regs, 0x1000u };
  regs.eax = 0x5650u;  
  call16_invoke(regs, 0x1a);
  if (regs.ax != 0x564Eu) {
    return installation_check_failed;
  }
  pxenvplus = address16{ 
    .off=regs.bx, 
    .seg=regs.es 
  }.as<PXENVplus*>();
  return success;
}

static void pxe_tftp_initialize()
{
  using namespace textio::fmt;
  using namespace pxenv;

  auto [status, cached_reply_s] = dhcp::info::cached(dhcp::packet_type_id::cached_reply);
  if (pxenv_status::success != status || !cached_reply_s.is_valid()) 
  {
    panick::pxenv_failed("Failed to get cached DHCP reply, or reply not valid.");
  }

#ifndef NO_DEBUG_LOGS
  format_to<"Cached reply packet : \n">(stdout);
  format_to<"  * {:.<19s} : {:d}\n"  >(stdout, "cached_reply_s.size", cached_reply_s.size());
  format_to<"  * {:.<19s} : {:08x}\n">(stdout, "cached_reply_s.data", cached_reply_s.data());
#endif  

#ifndef NO_DEBUG_LOGS
  format_to<"  * {:.<19s} : {}\n">(stdout, "client IP ",  cached_reply_s.client_ip());
  format_to<"  * {:.<19s} : {}\n">(stdout, "your IP ",    cached_reply_s.your_ip());
  format_to<"  * {:.<19s} : {}\n">(stdout, "server IP ",  cached_reply_s.server_ip());
  format_to<"  * {:.<19s} : {}\n">(stdout, "gateway IP ", cached_reply_s.gateway_ip());
  format_to<"  * {:.<19s} : {}\n">(stdout, "client MAC ", cached_reply_s.client_addr());
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

static auto pxe_initialize(PXENVplus& pxenvplus_s) -> version<std::uint8_t, std::uint8_t>
{
  using namespace textio::fmt;
  using namespace x86arch;

  if (!validate_PXENVplus(pxenvplus_s)) {
    panick::invalid_pxenvplus();
  }

  if (pxenvplus_s.version >= 0x201u)
  {
  #ifndef NO_DEBUG_LOGS
    format_to<"Using !PXE\n">(stdout);
  #endif

    auto bangpxe_p = address16 {
      .off = pxenvplus_s.pxe_bang_ptr.off, 
      .seg = pxenvplus_s.pxe_bang_ptr.seg
    }.as<bangPXE*>();

    if (!validate_bangPXE(*bangpxe_p))
    {
      panick::invalid_bangpxe();
    }      
    G_pxe_entry_point = bangpxe_p->entry_point_16;
  }
  else 
  {
  #ifndef NO_DEBUG_LOGS
    format_to<"Using PXENV+\n">(stdout);
  #endif
    G_pxe_entry_point = pxenvplus_s.entry_point_16;
  }
#ifndef NO_DEBUG_LOGS
  format_to<"PXE entry point : {}\n">(stdout, G_pxe_entry_point);
#endif
  return version<>::from_word<8, 8>(pxenvplus_s.version);
}

void pxenv::initialize(bool first_time)
{
  using namespace textio::fmt;
  if (!first_time)
    return;
  PXENVplus* pxenvplus_p{ nullptr };
  if (auto status = pxe_installation_check(pxenvplus_p); pxenv::pxenv_status::success != status) {
    panick::pxenv_failed("Failed to initialize PXE, installation check failed.");
  }
  [[maybe_unused]] const auto pxe_version = pxe_initialize(*pxenvplus_p);  
#ifndef NO_DEBUG_LOGS
  format_to<"Initialized PXE v{} ...\n">(stdout, pxe_version);
#endif
  pxe_tftp_initialize();
}

void pxenv::finalize(bool last_time)
{
  if(!last_time)
    return;
}

auto pxenv::call(void* params, std::uint16_t opcode) -> pxenv_status
{
  using namespace x86arch;
  auto address_of_params = address16::from(params);
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