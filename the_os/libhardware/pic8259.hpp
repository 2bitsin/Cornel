#pragma once

#include <cstddef>
#include <cstdint>

struct pic8259
{


  static void configure  (std::uint8_t offset_a, std::uint8_t offset_b);
  static void mask_or     (std::uint16_t m);
  static void mask_and    (std::uint16_t m);
  static auto read_mask   () -> std::uint16_t;
  static void write_mask  (std::uint16_t m);
};

