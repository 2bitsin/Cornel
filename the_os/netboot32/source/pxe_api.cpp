#include <string_view>

#include <hardware/console.hpp>
#include <hardware/x86arch.hpp>

#include <netboot32/pxe_api.hpp>
#include <netboot32/panick.hpp>

#include <memory/allocate_buffer.hpp>


static std::span<std::uint64_t> pxe_descriptor_table;

static void initialize(pxe_api::bangPXE& pxe_s)
{  
  using namespace x86arch;
  pxe_descriptor_table = allocate_buffer_of<std::uint64_t>(
    pxe_s.count_seg_desc, 
    [&pxe_s] (std::size_t index) 
    {
      const auto& desc = pxe_s.descriptors[index];    
      return make_32bit_segment({ 
        .type = segment_type::data,
        .base = desc.base,
        .size = desc.size
      });
    });
  lldt(pxe_descriptor_table);
  pxe_s.first_seg_sel = 0b100u; // start from LDT(0, RPL=0)
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
