#include <hardware/x86arch.hpp>

#include <memory/allocate_buffer.hpp>
#include <utils/bits.hpp>

#include <algorithm>
#include <ranges>
#include <utility>

using namespace x86arch;

auto x86arch::gdt_descriptor(gdt_descriptor_type params) -> std::uint64_t
{
  using namespace std;

  uint8_t flags { 0u };
  uint32_t size { params.size };
  uint32_t base { params.base };

  if (params.is_32bit)
  { flags |= 0b0100u; } 
  else 
  { 
    size = std::clamp(size, 0u, 0x00ffffu); 
    base = std::clamp(base, 0u, 0xffffffu);
  }
     
  if (size >= 0x100000u) 
  { 
    size = (size >> 12u) + !!(size & 0xfffu);
    size = std::clamp(size, 0u, 0xfffffu);    
    flags |= 0b1000u;    
  }

  const uint8_t access { ([&params] () -> uint8_t
  {
    const auto p = params.is_present ? 1u : 0u;
    const auto l = std::clamp(1u*params.priv_level, 0u, 3u);
    using enum segment_type;
    switch (params.type)
    {
    default    :
    case data  : return bits::pack<5, 2, 1> (0b10010u, l, p);
    case stack : return bits::pack<5, 2, 1> (0b10110u, l, p);
    case code  : return bits::pack<5, 2, 1> (0b11110u, l, p);
    }
  })() };
    
  const auto [size_l, size_h] = bits::unpack_as_tuple<16, 16>(size);
  const auto [base_l, base_h] = bits::unpack_as_tuple<24, 8>(base);

  return bits::pack<16, 24, 8, 4, 4, 8>(size_l, base_l, access, size_h, flags, base_h);
}


auto x86arch::gdt_table_resize(std::uint16_t new_size, std::uint64_t defval, std::uint32_t flags) -> std::span<std::uint64_t>
{
  using textio::simple::fmt::hex;
  x86arch::interrupt_guard cli;
  auto curr_gdt = x86arch::sgdt(); 
  curr_gdt = reallocate_buffer(curr_gdt, new_size, defval, flags);
  x86arch::lgdt(curr_gdt);
  return curr_gdt;
}

auto x86arch::gdt_descriptor_set(std::uint16_t index, std::uint64_t desc) 
  -> std::uint64_t 
{
  x86arch::interrupt_guard cli;
  return std::exchange(sgdt()[index], desc);
}

auto x86arch::gdt_descriptor_get(std::uint16_t index) -> std::uint64_t
{
  x86arch::interrupt_guard cli;
  return sgdt()[index];
}

auto x86arch::gdt_selector(std::uint16_t index, std::uint8_t rpl) -> std::uint16_t
{
  return bits::pack<2, 1, 13>(rpl, 0, index);
}

auto x86arch::gdt_table_size() -> std::uint16_t
{
  return sgdt().size();
}