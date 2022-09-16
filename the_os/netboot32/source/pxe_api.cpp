#include <string_view>

#include <netboot32/pxe_api.hpp>
#include <netboot32/panick.hpp>
#include <hardware/console.hpp>

#include <misc/debug.hpp>

static auto validate_bangPXE(pxe_api::bangPXE const& s) -> bool
{
  using namespace std::string_view_literals;
  std::string_view signature_view(s.signature, std::size(s.signature));
  if (signature_view != "!PXE"sv) 
    return false;
  std::span byte_span_s{ (std::uint8_t const*)&s, s.length };
  std::uint8_t byte_based_sum{ 0u };
  for (auto&& single_byte : byte_span_s)
    byte_based_sum += single_byte;
  if (0u != byte_based_sum)
    return false; 
  return true;
}

static auto validate_PXENVplus(pxe_api::PXENVplus const& s) -> bool
{
  using namespace std::string_view_literals;
  std::string_view signature_view(s.signature, std::size(s.signature));
  if (signature_view != "PXENV+"sv) 
    return false;
  std::span byte_span_s{ (std::uint8_t const*)&s, s.length };
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

  if (context.m_PXENVplus.version >= 0x201)
  {
    if (!validate_bangPXE(context.m_bangPXE))
    {
      panick::invalid_bangpxe();
      return;
    }


    initialize_descriptors(context.m_bangPXE);

    return;
  }

  //if (validate_bangPXE(context.m_bangPXE))
  //{
//
  //}

}

void pxe_api::finalize(bool last_time, initialize_context const&)
{
  if(!last_time)
    return;
}
