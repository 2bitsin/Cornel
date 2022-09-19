#include <string_view>
#include <span>
#include <cstddef>

#include <hardware/console.hpp>
#include <hardware/x86arch.hpp>

#include <netboot32/pxe_api.hpp>
#include <netboot32/panick.hpp>

#include <memory/allocate_buffer.hpp>

#include <utils/bits.hpp>

#pragma pack(push, 1)

struct segoff32
{
  std::uint32_t off;
  std::uint16_t seg;
};

struct segoff
{   
  std::uint16_t off;  
  std::uint16_t seg;
   
  template <typename T = void> 
  constexpr auto ptr() const noexcept -> T* 
  { return (T*)(seg*16ul + off); }
};

struct segdesc32
{
  uint16_t real_seg; 
  uint32_t base;
  uint16_t size;
};

struct segdesc16
{
  uint16_t real_seg;
  uint16_t size;
};

struct PXENVplus
{
  char      signature[6];    
  uint16_t  version;
  uint8_t   length;
  uint8_t   checksum;
  segoff    entry_point_16;
  uint32_t  entry_point_32;
  uint16_t  entry_point_32_seg;
  segdesc16 seg_desc[5];
  segoff    pxe_bang_ptr;
};

struct bangPXE
{
  char      signature[4];
  uint8_t   length;
  uint8_t   checksum;
  uint8_t   revision;
  uint8_t   reserved0;
  segoff    undi_rom_id;
  segoff    base_rom_id;
  segoff    entry_point_16;
  segoff    entry_point_32;
  segoff    status_callback_16;
  uint8_t   reserved1;
  uint8_t   count_seg_desc;
  uint16_t  first_seg_sel;
  // Can be any number of segdescs
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wpedantic"  
  segdesc32 seg_desc[0];
#pragma GCC diagnostic pop
};
#pragma pack(pop)


static_assert(offsetof(bangPXE, signature         ) == 0x00);
static_assert(offsetof(bangPXE, length            ) == 0x04);
static_assert(offsetof(bangPXE, checksum          ) == 0x05);
static_assert(offsetof(bangPXE, revision          ) == 0x06);
static_assert(offsetof(bangPXE, reserved0         ) == 0x07);
static_assert(offsetof(bangPXE, undi_rom_id       ) == 0x08);
static_assert(offsetof(bangPXE, base_rom_id       ) == 0x0c);
static_assert(offsetof(bangPXE, entry_point_16    ) == 0x10);
static_assert(offsetof(bangPXE, entry_point_32    ) == 0x14);
static_assert(offsetof(bangPXE, status_callback_16) == 0x18);
static_assert(offsetof(bangPXE, reserved1         ) == 0x1c);
static_assert(offsetof(bangPXE, count_seg_desc    ) == 0x1d);
static_assert(offsetof(bangPXE, first_seg_sel     ) == 0x1e);
static_assert(offsetof(bangPXE, seg_desc[0]       ) == 0x20);
static_assert(offsetof(bangPXE, seg_desc[1]       ) == 0x28);
static_assert(offsetof(bangPXE, seg_desc[2]       ) == 0x30);
static_assert(offsetof(bangPXE, seg_desc[3]       ) == 0x38);
static_assert(offsetof(bangPXE, seg_desc[4]       ) == 0x40);
static_assert(offsetof(bangPXE, seg_desc[5]       ) == 0x48);
static_assert(offsetof(bangPXE, seg_desc[6]       ) == 0x50);

using PXE_entry_point_t = void __attribute__((cdecl))(std::uint16_t, void*);

static segoff32 G_pxe_entry_point{ 0, 0 };

static void initialize([[maybe_unused]] bangPXE& pxe_s)
{  
  using namespace x86arch;  
  console::writeln("Using !PXE interface.");
  const auto initial_table_size = x86arch::gdt_table_size();
  x86arch::gdt_table_resize(initial_table_size + pxe_s.count_seg_desc);
  pxe_s.first_seg_sel = x86arch::gdt_selector((std::uint16_t)initial_table_size);
  G_pxe_entry_point.seg = pxe_s.entry_point_32.seg;
  G_pxe_entry_point.off = pxe_s.entry_point_32.off;

  for(std::size_t i = 0u; i < pxe_s.count_seg_desc; ++i)
  {    
    x86arch::segment_type type;

    using enum x86arch::segment_type;
    switch(i)
    {
    case 0 : type = data; break;  // Stack
    case 1 : type = data; break;  // UNDI Data
    case 2 : type = code; break;  // UNDI Code
    case 3 : type = data; break;  // UNDI Code Write
    case 4 : type = data; break;  // BC Data
    case 5 : type = code; break;  // BC Code
    case 6 : type = data; break;  // BC Code Write
    default: type = data; break;  // BC Code Write
    }

    x86arch::gdt_descriptor_set(
      initial_table_size + i, 
      gdt_descriptor({
        .type = type,
        .base = pxe_s.seg_desc[i].base,
        .size = pxe_s.seg_desc[i].size,
        .is_32bit = false
      }));     
    if (pxe_s.seg_desc[i].real_seg == pxe_s.entry_point_32.seg && type == code)
      G_pxe_entry_point.seg = x86arch::gdt_selector((std::uint16_t)(initial_table_size + i));
  }
}

static void initialize([[maybe_unused]] PXENVplus& pxe_s)
{
  using namespace x86arch;  
  console::writeln("Using PXENV+ interface.");
  const auto initial_table_size = x86arch::gdt_table_size();
  G_pxe_entry_point.seg = pxe_s.entry_point_32_seg;
  G_pxe_entry_point.off = pxe_s.entry_point_32;
  x86arch::gdt_table_resize(initial_table_size + std::size(pxe_s.seg_desc));
  for (std::size_t i = 0u; i < std::size(pxe_s.seg_desc); ++i)
  {
    x86arch::segment_type type;

    using enum x86arch::segment_type;
    switch (i)
    {
    case 0  : type = data; break;  // Stack
    case 1  : type = code; break;  // BC code
    case 2  : type = data; break;  // BC data
    case 3  : type = data; break;  // UNDI data
    case 4  : type = code; break;  // UNDI code    
    default : type = data; break;  // other
    }

    x86arch::gdt_descriptor_set(
      initial_table_size + i, 
      gdt_descriptor({
        .type = type,
        .base = pxe_s.seg_desc[i].real_seg*16u,
        .size = pxe_s.seg_desc[i].size
      }));

    auto current_selector = x86arch::gdt_selector(initial_table_size + i);;
    if (pxe_s.seg_desc[i].real_seg == G_pxe_entry_point.seg && type == code) {      
      G_pxe_entry_point.seg = current_selector;
    }
    pxe_s.seg_desc[i].real_seg = current_selector;
  }
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

auto pxe_api::get_cached_info(std::uint16_t packet_type, std::span<const std::byte>& buffer, std::uint16_t& buffer_limit) -> std::uint16_t
{
  using namespace textio::simple::fmt;

  struct s_PXENV_GET_CACHED_INFO
  {
    std::uint16_t status;
    std::uint16_t packet_type;

    std::uint16_t buffer_size;
    segoff        buffer;
    std::uint16_t buffer_limit;
  };  

  s_PXENV_GET_CACHED_INFO params = 
  {
    .status       = 0u,
    .packet_type  = packet_type,
    .buffer_size  = (std::uint16_t)(std::clamp(buffer.size(), 0u, 0xFFFFu)),
    .buffer       = { 0u, 0u },
    .buffer_limit = buffer_limit  
  };

  console::writeln("entry_point_32=(", hex<'&'>(G_pxe_entry_point.seg), ", ", hex<'&'>(G_pxe_entry_point.off), ')'); 

  __debugbreak();
  __asm__ volatile ("lcall %0" : : "m"(G_pxe_entry_point));
  __debugbreak();

  if (params.status != 0u) 
    return params.status;

  buffer = std::span<const std::byte> { params.buffer.ptr<const std::byte>(), params.buffer_size };

  buffer_limit = params.buffer_limit;
  return 0u;
}

