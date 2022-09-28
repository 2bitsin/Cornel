#pragma once

#include <concepts>
#include <utils/bits.hpp>

template <std::integral Maj = std::uint8_t, std::integral Min = std::uint8_t>
struct version
{
  const Maj major;
  const Min minor;

  constexpr version(Maj major, Min minor) noexcept
  : major(major), minor(minor) 
  {}

  friend constexpr inline auto operator == (version<Maj, Min> const& lhs, version<Maj, Min> const& rhs) noexcept -> bool
  { return lhs.major == rhs.major && lhs.minor == rhs.minor; }

  friend constexpr inline auto operator != (version<Maj, Min> const& lhs, version<Maj, Min> const& rhs) noexcept -> bool
  { return !(lhs == rhs); }

  friend constexpr inline auto operator <=> (version<Maj, Min> const& lhs, version<Maj, Min> const& rhs) noexcept -> std::strong_ordering
  {
    if (lhs.major == rhs.major)
      return lhs.minor <=> rhs.minor;
    return lhs.major <=> rhs.major;
  }

  template <std::size_t szMaj, std::size_t szMin, typename VersionWord>
  static constexpr auto from_word(VersionWord ver_word) noexcept -> version
  {     
    auto [minor, major] = bits::unpack_as_tuple<szMin, szMaj>(ver_word);
    return version<decltype(major), decltype(minor)>{ major, minor };
  }

  template <auto I>
  constexpr inline auto get() noexcept
  {
    if constexpr (0u == I)
      return major;
    else if constexpr (1u == I)
      return minor;
    else
      static_assert(I > 2u, "Index out of bounds");
  }
#ifdef __LIBTEXTIO__
  template <std::output_iterator<char> O>
  auto write (O out_i) const noexcept -> O
  {    
    return textio::simple::write(out_i, major, '.', minor);
  }
#endif
};

namespace std
{
  template <std::integral Maj, std::integral Min>
  struct tuple_size<::version<Maj, Min>>: std::integral_constant<std::size_t, 2> {};

  template <std::integral Maj, std::integral Min>
  struct tuple_element<0, ::version<Maj, Min>>: std::type_identity<Maj> {};

  template <std::integral Maj, std::integral Min>
  struct tuple_element<1, ::version<Maj, Min>>: std::type_identity<Min> {};
}