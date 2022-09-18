#pragma once

#include <cstddef>
#include <cstdint>

namespace pic8259
{
  void configure (std::uint8_t offset_a, std::uint8_t offset_b);
  void mask_or (std::uint16_t m);
  void mask_and (std::uint16_t m);
  auto read_mask () -> std::uint16_t;
  void write_mask (std::uint16_t m);
  void end_of_interrupt (std::uint8_t irq);
};

