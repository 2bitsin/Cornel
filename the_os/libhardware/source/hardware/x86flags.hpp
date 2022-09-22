#pragma once
namespace x86arch::flags
{
  static inline const constexpr std::uint16_t carry      = 0b0000'0000'0000'0000'0000'0000'0000'0001u;
  static inline const constexpr std::uint16_t parity     = 0b0000'0000'0000'0000'0000'0000'0000'0100u;
  static inline const constexpr std::uint16_t adjust     = 0b0000'0000'0000'0000'0000'0000'0001'0000u;
  static inline const constexpr std::uint16_t zero       = 0b0000'0000'0000'0000'0000'0000'0100'0000u;
  static inline const constexpr std::uint16_t sign       = 0b0000'0000'0000'0000'0000'0000'1000'0000u;
  static inline const constexpr std::uint16_t trap       = 0b0000'0000'0000'0000'0000'0001'0000'0000u;
  static inline const constexpr std::uint16_t interrupt  = 0b0000'0000'0000'0000'0000'0010'0000'0000u;
  static inline const constexpr std::uint16_t direction  = 0b0000'0000'0000'0000'0000'0100'0000'0000u;
  static inline const constexpr std::uint16_t overflow   = 0b0000'0000'0000'0000'0000'1000'0000'0000u;
  static inline const constexpr std::uint16_t iopl       = 0b0000'0000'0000'0000'0011'0000'0000'0000u;
  static inline const constexpr std::uint16_t nestedtask = 0b0000'0000'0000'0000'0100'0000'0000'0000u;
  static inline const constexpr std::uint32_t resume     = 0b0000'0000'0000'0001'0000'0000'0000'0000u;
  static inline const constexpr std::uint32_t virtual_86 = 0b0000'0000'0000'0010'0000'0000'0000'0000u;
  static inline const constexpr std::uint32_t alignment  = 0b0000'0000'0000'0100'0000'0000'0000'0000u;
  static inline const constexpr std::uint32_t virtual_if = 0b0000'0000'0000'1000'0000'0000'0000'0000u;
  static inline const constexpr std::uint32_t virtual_ip = 0b0000'0000'0001'0000'0000'0000'0000'0000u;
  static inline const constexpr std::uint32_t cpuid      = 0b0000'0000'0010'0000'0000'0000'0000'0000u;

} // namespace name
