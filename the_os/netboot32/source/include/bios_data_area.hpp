#pragma once

#include <utilities.hpp>

auto&& BDA_com_port_address       = variable_at<0x0400u, volatile std::uint16_t [4]>;
auto&& BDA_lpt_port_address       = variable_at<0x0408u, volatile std::uint16_t [4]>;

auto&& BDA_bios_display_mode      = variable_at<0x0449u, volatile std::uint8_t>;
auto&& BDA_number_of_columns      = variable_at<0x044Au, volatile std::uint16_t>;
auto&& BDA_last_row_number        = variable_at<0x0484u, volatile std::uint16_t>;
auto&& BDA_size_of_video_page     = variable_at<0x044Cu, volatile std::uint16_t>;
auto&& BDA_offset_of_video_page   = variable_at<0x044Eu, volatile std::uint16_t>;

auto&& BDA_page_cursor_position   = variable_at<0x0450u, volatile std::uint8_t [8][2]>;
auto&& BDA_cursor_shape           = variable_at<0x0460u, volatile std::uint16_t>;
auto&& BDA_active_video_page      = variable_at<0x0462u, volatile std::uint8_t>;
auto&& BDA_video_adapter_io_port  = variable_at<0x0463u, volatile std::uint16_t>;
auto&& BDA_video_adapter_mode_reg = variable_at<0x0465u, volatile std::uint8_t>;

