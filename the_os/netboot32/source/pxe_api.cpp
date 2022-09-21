#include <string_view>
#include <span>
#include <cstddef>

#include <hardware/console.hpp>
#include <hardware/x86gdt.hpp>

#include <netboot32/pxe_api.hpp>
#include <netboot32/panick.hpp>
#include <netboot32/pxe_defs.hpp>

#include <memory/allocate_buffer.hpp>

#include <utils/bits.hpp>


static void initialize([[maybe_unused]] bangPXE& pxe_s)
{}

static void initialize([[maybe_unused]] PXENVplus& pxe_s)
{}

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

void pxe_api::initialize(bool first_time, initialize_context const& context)
{
  if(!first_time)  
    return;

  if (!validate_PXENVplus(context.m_PXENVplus))
  {
    panick::invalid_pxenvplus();
    return;
  }

  auto[minor, major] = bits::unpack_as_tuple<8, 8>(context.m_PXENVplus.version);
  using namespace textio::simple::fmt;

  console::writeln("Initializing PXE v", major, '.', minor, " ... ");
  if (context.m_PXENVplus.version >= 0x201)
  {
    if (!validate_bangPXE(context.m_bangPXE))
    {
      panick::invalid_bangpxe();
      return;
    }
    ::initialize(context.m_bangPXE);
    return;
  }
  ::initialize(context.m_PXENVplus);
}

void pxe_api::finalize(bool last_time, initialize_context const&)
{
  if(!last_time)
    return;
}

auto pxe_api::get_cached_info([[maybe_unused]] std::uint16_t packet_type, [[maybe_unused]] std::span<const std::byte>& buffer, [[maybe_unused]] std::uint16_t& buffer_limit) -> std::uint16_t
{
  using namespace textio::simple::fmt;

  return 0u;
}

