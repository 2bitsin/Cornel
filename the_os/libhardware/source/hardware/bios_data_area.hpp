#pragma once

#include <utils/var.hpp>

namespace bda
{
  static inline auto&& com_port_address         = variable_at<0x0400u, volatile std::uint16_t [4]>;
  static inline auto&& lpt_port_address         = variable_at<0x0408u, volatile std::uint16_t [4]>;
  static inline auto&& conventional_memory_size = variable_at<0x0413u, volatile std::uint16_t>;
  static inline auto&& bios_display_mode        = variable_at<0x0449u, volatile std::uint8_t>;
  static inline auto&& number_of_columns        = variable_at<0x044Au, volatile std::uint16_t>;
  static inline auto&& last_row_number          = variable_at<0x0484u, volatile std::uint8_t>;
  static inline auto&& size_of_video_page       = variable_at<0x044Cu, volatile std::uint16_t>;
  static inline auto&& offset_of_video_page     = variable_at<0x044Eu, volatile std::uint16_t>;
  static inline auto&& page_cursor_position     = variable_at<0x0450u, volatile std::uint8_t [8][2]>;
  static inline auto&& cursor_shape             = variable_at<0x0460u, volatile std::uint16_t>;
  static inline auto&& active_video_page        = variable_at<0x0462u, volatile std::uint8_t>;
  static inline auto&& video_adapter_io_port    = variable_at<0x0463u, volatile std::uint16_t>;
  static inline auto&& video_adapter_mode_reg   = variable_at<0x0465u, volatile std::uint8_t>;
};