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


struct PXENV_context
{
  x86arch::address16 entry_point { 0u, 0u };
  std::uint16_t version { 0u };
  bool use_bange_pxe { false };
};

static PXENV_context G_PXENV_context;

static auto PXENV_validate_bangPXE(bangPXE const& pxe_s) -> bool
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

static auto PXENV_validate_PXENVplus(PXENVplus const& pxe_s) -> bool
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

static auto PXENV_installation_check() 
  -> std::tuple<pxenv::pxenv_status, PXENVplus*> 
{
  using namespace x86arch;
  using enum pxenv::pxenv_status;

  call16_context regs;
  call16_stack stack { regs, 0x1000u };
  regs.eax = 0x5650u;  
  call16_invoke(regs, 0x1a);
  if (regs.ax != 0x564Eu) {
    return { installation_check_failed, nullptr };
  } 
  auto const pxenvplus_p = address16{ .off=regs.bx, .seg=regs.es }.as<PXENVplus*>();
  return { success, pxenvplus_p };
}

static auto PXENV_initialize(PXENV_context& context_v, PXENVplus& pxenvplus_s) -> pxenv_status
{
  using x86arch::address16;
  
  if (!PXENV_validate_PXENVplus(pxenvplus_s)) {
    return pxenv_status::bad_pxenvplus_structure;
  }

  pxenv_status status { pxenv_status::success };  

  context_v.version = pxenvplus_s.version;  
  if (pxenvplus_s.version >= 0x201u)
  {
    auto bangpxe_p = address16::from(pxenvplus_s.bangpxe_addr).as<bangPXE*>();
    if (nullptr != bangpxe_p && PXENV_validate_bangPXE(*bangpxe_p)) 
    {
      context_v.entry_point = bangpxe_p->entry_point_16;
      context_v.use_bange_pxe = true;      
      return pxenv_status::success;      
    }
    status = pxenv_status::bad_bangpxe_structure;
    // Something went wrong with !PXE struct, falling back to PXENV+
  }  
  context_v.entry_point = pxenvplus_s.entry_point_16;
  context_v.use_bange_pxe = false;  
  return status;
}

void pxenv::initialize(bool first_time)
{
  using namespace textio::fmt;
  using std::tie;

  if (!first_time)
    return;

  PXENVplus* pxenvplus_p { nullptr };
  pxenv_status status { pxenv_status::failure };

  // Clear G_PXENV_context
  G_PXENV_context = PXENV_context{};

  // Check if PXE installed and enabled
  tie(status, pxenvplus_p) = PXENV_installation_check();
  if (pxenv_status::success != status) {
    panick::pxenv_error("Failed to initialize PXE, installation check failed.");  
  }

  // Initialize PXE
  status = PXENV_initialize(G_PXENV_context, *pxenvplus_p);
  if (pxenv_status::bad_bangpxe_structure == status) 
  {
  #ifndef NO_DEBUG_LOGS
    format_to<"Warning: !PXE structure is invalid, falling back to PXENV+ structure.\n">(stdout);
  #endif
  } 
  else if (pxenv_status::success != status) 
  {
    panick::pxenv_error("failed to initialize PXE, PXENV+ structure is invalid.");
  }

  // Try to grab DHCP parameters
  {
    const auto [status, cached_reply_s] = dhcp::info::cached(dhcp::packet_type_id::cached_reply);
    if (pxenv_status::success != status || !cached_reply_s.is_valid()) 
    {  
      panick::pxenv_error("failed to get cached DHCP reply, or reply not valid.");
    }

  #ifndef NO_DEBUG_LOGS
    format_to<"Initialized PXE v{}.{} ...\n">(stdout, (G_PXENV_context.version >> 8) & 0xff, G_PXENV_context.version & 0xff);
    format_to<"  * {:.<19s} : {} ({:s})\n">(stdout, "Entry point", G_PXENV_context.entry_point, G_PXENV_context.use_bange_pxe ? "!PXE" : "PXENV+");
    format_to<"  * {:.<19s} : {}\n">(stdout, "Client IP ",  cached_reply_s.client_ip());
    format_to<"  * {:.<19s} : {}\n">(stdout, "Your IP ",    cached_reply_s.your_ip());
    format_to<"  * {:.<19s} : {}\n">(stdout, "Server IP ",  cached_reply_s.server_ip());
    format_to<"  * {:.<19s} : {}\n">(stdout, "Gateway IP ", cached_reply_s.gateway_ip());
    format_to<"  * {:.<19s} : {}\n">(stdout, "Client MAC ", cached_reply_s.client_addr());    
  #endif 
  }  
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
  call16_invoke(ctx, G_PXENV_context.entry_point);
  return static_cast<pxenv_status>(ctx.ax);
}