#include <string_view>

#include <hardware/console.hpp>
#include <hardware/x86arch.hpp>

#include <netboot32/pxe_api.hpp>
#include <netboot32/panick.hpp>

#include <memory/allocate_buffer.hpp>

#include <misc/bits.hpp>


static std::span<std::uint64_t> pxe_descriptor_table;

static void initialize([[maybe_unused]] pxe_api::bangPXE& pxe_s)
{  
  using namespace x86arch;  
  
}

static void initialize([[maybe_unused]] pxe_api::PXENVplus& pxe_s)
{

}

static auto validate_bangPXE(pxe_api::bangPXE const& pxe_s) -> bool
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

static auto validate_PXENVplus(pxe_api::PXENVplus const& pxe_s) -> bool
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
  console::writeln("prot_mode_stack_seg  : ", hex<'&'>(context.m_PXENVplus.prot_mode_stack_seg));
  console::writeln("prot_mode_stack_size : ", hex<'&'>(context.m_PXENVplus.prot_mode_stack_size));
  console::writeln("base_code_seg        : ", hex<'&'>(context.m_PXENVplus.base_code_seg));
  console::writeln("base_code_size       : ", hex<'&'>(context.m_PXENVplus.base_code_size));
  console::writeln("base_data_seg        : ", hex<'&'>(context.m_PXENVplus.base_data_seg));
  console::writeln("base_data_size       : ", hex<'&'>(context.m_PXENVplus.base_data_size));
  console::writeln("undi_data_seg        : ", hex<'&'>(context.m_PXENVplus.undi_data_seg));
  console::writeln("undi_data_size       : ", hex<'&'>(context.m_PXENVplus.undi_data_size));
  console::writeln("undi_code_seg        : ", hex<'&'>(context.m_PXENVplus.undi_code_seg));
  console::writeln("undi_code_size       : ", hex<'&'>(context.m_PXENVplus.undi_code_size));

  

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
