#pragma once

#include <cstdint>
#include <cstddef>

namespace kbdctrl
{
  static inline const constexpr std::uint8_t status_port                     = 0x64u;
  static inline const constexpr std::uint8_t command_port                    = 0x64u;
  static inline const constexpr std::uint8_t data_port                       = 0x60u;  
  
  static inline const constexpr std::uint8_t status_output_buffer_full_bit   = 0x01u;
  static inline const constexpr std::uint8_t status_input_buffer_full_bit    = 0x02u;

  static inline const constexpr std::uint8_t command_disable_keyboard        = 0xadu;
  static inline const constexpr std::uint8_t command_enable_keyboard         = 0xaeu;
  static inline const constexpr std::uint8_t command_read_input_port         = 0xc0u;
  static inline const constexpr std::uint8_t command_read_output_port        = 0xd0u;
  static inline const constexpr std::uint8_t command_write_output_port       = 0xd1u;
  static inline const constexpr std::uint8_t command_read_configuration      = 0x20u;
  static inline const constexpr std::uint8_t command_write_configuration     = 0x60u;
  static inline const constexpr std::uint8_t command_self_test               = 0xaau;
  static inline const constexpr std::uint8_t command_interface_test          = 0xabu;
  static inline const constexpr std::uint8_t command_read_test_inputs        = 0xe0u;
  static inline const constexpr std::uint8_t command_pulse_output_lines      = 0xf0u;

  static inline const constexpr std::uint8_t output_port_a20_enable_bit      = 0x02u;
  static inline const constexpr std::uint8_t line_reset                      = 0x01u;
  static inline const constexpr std::uint8_t wait                            = 0x01u;
  static inline const constexpr std::uint8_t dont_wait                       = 0x00u;

  void hardware_reset();
  void send_command(std::uint8_t value, std::uint8_t wait);
  auto read_data(std::uint8_t wait) -> std::uint8_t;
  void send_data(std::uint8_t value, std::uint8_t wait);
  void pulse_output_lines(std::uint8_t value);
  void disable_keyboard();
  void enable_keyboard();
  auto read_output_port() -> std::uint8_t;
  void send_output_port(std::uint8_t value);
  void wait_for_output_buffer_not_empty();
  void wait_for_input_buffer_empty();
};
