#pragma once

#include <cstdint>
#include <cstddef>


struct kbdctrl
{
  static inline const constexpr std::uint8_t STATUS_PORT                     = 0x64u;
  static inline const constexpr std::uint8_t COMMAND_PORT                    = 0x64u;
  static inline const constexpr std::uint8_t DATA_PORT                       = 0x60u;  
  
  static inline const constexpr std::uint8_t STATUS_OUTPUT_BUFFER_FULL_BIT   = 0x01u;
  static inline const constexpr std::uint8_t STATUS_INPUT_BUFFER_FULL_BIT    = 0x02u;

  static inline const constexpr std::uint8_t COMMAND_DISABLE_KEYBOARD        = 0xADu;
  static inline const constexpr std::uint8_t COMMAND_ENABLE_KEYBOARD         = 0xAEu;
  static inline const constexpr std::uint8_t COMMAND_READ_INPUT_PORT         = 0xC0u;
  static inline const constexpr std::uint8_t COMMAND_READ_OUTPUT_PORT        = 0xD0u;
  static inline const constexpr std::uint8_t COMMAND_WRITE_OUTPUT_PORT       = 0xD1u;
  static inline const constexpr std::uint8_t COMMAND_READ_CONFIGURATION      = 0x20u;
  static inline const constexpr std::uint8_t COMMAND_WRITE_CONFIGURATION     = 0x60u;
  static inline const constexpr std::uint8_t COMMAND_SELF_TEST               = 0xAAu;
  static inline const constexpr std::uint8_t COMMAND_INTERFACE_TEST          = 0xABu;
  static inline const constexpr std::uint8_t COMMAND_READ_TEST_INPUTS        = 0xE0u;
  static inline const constexpr std::uint8_t COMMAND_PULSE_OUTPUT_LINES      = 0xF0u;

  static inline const constexpr std::uint8_t OUTPUT_PORT_A20_ENABLE_BIT      = 0x02u;
  static inline const constexpr std::uint8_t LINE_RESET                      = 0x01u;
  static inline const constexpr std::uint8_t WAIT                            = 0x01u;
  static inline const constexpr std::uint8_t DONT_WAIT                       = 0x00u;


  static void hardware_reset();
  static void send_command(std::uint8_t value, std::uint8_t wait);
  static auto read_data(std::uint8_t wait) -> std::uint8_t;
  static void send_data(std::uint8_t value, std::uint8_t wait);
  static void pulse_output_lines(std::uint8_t value);
  static void disable_keyboard();
  static void enable_keyboard();
  static auto read_output_port() -> std::uint8_t;
  static void send_output_port(std::uint8_t value);
  static void wait_for_output_buffer_not_empty();
  static void wait_for_input_buffer_empty();
};
