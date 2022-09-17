#include <hardware/x86arch.hpp>
#include <misc/utilities_bits.hpp>
#include <algorithm>

using namespace x86arch;

auto x86arch::make_32bit_segment(make_32bit_segment_t params) -> std::uint64_t
{
  using namespace std;

  auto Gbit{ params.size >= 0x100000u };
  auto size{ params.size };

  if (Gbit) 
  { 
    size = (size >> 12u) + !!(size & 0xfffu);
    size = std::clamp(size, 0u, 0xfffffu);
  }

  const uint8_t access { ([&params] () -> uint8_t
  {
    const auto p = params.present ? 1u : 0u;
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

  const uint8_t flags = Gbit ? 0x8u : 0x0u ;
    
  const auto [size_l, size_h] = bits::unpack_as_tuple<16, 16>(size);
  const auto [base_l, base_h] = bits::unpack_as_tuple<24, 8>(params.base);

  return bits::pack<16, 24, 8, 4, 4, 8>(size_l, base_l, access, size_h, flags, base_h);
}
