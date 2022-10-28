#pragma once

#include <cstddef>
#include <cstdint>
#include <new>

namespace x86arch
{
  #pragma pack(push, 1)
  struct alignas(16) bios_acpi_memory_map_entry_t
  {
    std::uint64_t base;
    std::uint64_t size;
    std::uint32_t type;
    std::uint32_t attr;
  };  

  struct bios_data_area_layout
  {
    std::uint16_t com_port_base[4];
    std::uint16_t lpt_port_base[4];

  // ----

    std::uint16_t equipment_bits;
    std::uint8_t  interrupt_flag;
    std::uint16_t base_memory_size;
    std::uint16_t adapter_memory_size;

  // ----

    std::uint16_t keyboard_shift_flags;
    std::uint8_t  alt_numpad_work_area;
    std::uint16_t keyboard_buffer_head;
    std::uint16_t keyboard_buffer_tail;
    std::uint8_t  keyboard_buffer[32u];

  // ----

    std::uint8_t  floppy_drive_calibration_status;
    std::uint8_t  floppy_drive_motor_status;
    std::uint8_t  floppy_drive_motor_timeout;
    std::uint8_t  floppy_drive_status;
    std::uint8_t  drive_controller_status_register[3];
    std::uint8_t  floppy_disk_cylinder_number;
    std::uint8_t  floppy_disk_head_number;
    std::uint8_t  floppy_disk_sector_number;
    std::uint8_t  floppy_disk_bytes_written;    

  //---- 

    std::uint8_t  active_video_mode;
    std::uint16_t number_of_text_columns;
    std::uint16_t active_video_page_size;
    std::uint16_t active_video_page_offset;
    std::uint8_t  page_cursor_position[8u][2u];
    std::uint8_t  cursor_shape[2u];
    std::uint8_t  active_video_page;
    std::uint16_t video_io_port;
    std::uint8_t  video_mode_register;
    std::uint8_t  color_palette;
    std::uint16_t adapter_rom_offset;
    std::uint16_t adapter_rom_segment;

  //----

    std::uint8_t  last_interrupt_bit;
    std::uint32_t interrupt_1a_counter;
    std::uint8_t  timer_24h_flag;
    std::uint8_t  keyboard_ctrl_break_flag;
    std::uint16_t soft_reset_flag;

  //----

    std::uint8_t  last_harddisk_operation_status;
    std::uint8_t  number_of_harddisk_drives;
    std::uint8_t  harddisk_control_byte;
    std::uint8_t  harddisk_drive_port_offset;

  //----

    std::uint8_t  lpt_port_timeout[4u];
    std::uint8_t  com_port_timeout[4u];

  //----

    std::uint16_t keyboard_buffer_begin;
    std::uint16_t keyboard_buffer_end;

  //----

    std::uint8_t  number_of_text_rows_minus_one;
    std::uint16_t number_of_scanlines_per_character;
    std::uint8_t  video_adapter_options;

  //----

    std::byte _Reserved[0x100 - 0x88u];
  };

  static_assert(offsetof(bios_data_area_layout, com_port_base) == 0x00u);
  static_assert(offsetof(bios_data_area_layout, lpt_port_base) == 0x08u);
  static_assert(offsetof(bios_data_area_layout, equipment_bits) == 0x10u);
  static_assert(offsetof(bios_data_area_layout, base_memory_size) == 0x13u);
  static_assert(offsetof(bios_data_area_layout, adapter_memory_size) == 0x15u);
  static_assert(offsetof(bios_data_area_layout, keyboard_shift_flags) == 0x17u);
  static_assert(offsetof(bios_data_area_layout, alt_numpad_work_area) == 0x19u);
  static_assert(offsetof(bios_data_area_layout, keyboard_buffer_head) == 0x1au);
  static_assert(offsetof(bios_data_area_layout, keyboard_buffer_tail) == 0x1cu);
  static_assert(offsetof(bios_data_area_layout, keyboard_buffer) == 0x1eu);
  static_assert(offsetof(bios_data_area_layout, floppy_drive_calibration_status) == 0x3eu);
  static_assert(offsetof(bios_data_area_layout, floppy_drive_motor_status) == 0x3fu);
  static_assert(offsetof(bios_data_area_layout, floppy_drive_motor_timeout) == 0x40u);
  static_assert(offsetof(bios_data_area_layout, floppy_drive_status) == 0x41u);
  static_assert(offsetof(bios_data_area_layout, drive_controller_status_register) == 0x42u);
  static_assert(offsetof(bios_data_area_layout, floppy_disk_cylinder_number) == 0x45u);
  static_assert(offsetof(bios_data_area_layout, floppy_disk_head_number) == 0x46u);
  static_assert(offsetof(bios_data_area_layout, floppy_disk_sector_number) == 0x47u);
  static_assert(offsetof(bios_data_area_layout, floppy_disk_bytes_written) == 0x48u);
  static_assert(offsetof(bios_data_area_layout, active_video_mode) == 0x49u);
  static_assert(offsetof(bios_data_area_layout, number_of_text_columns) == 0x4au);
  static_assert(offsetof(bios_data_area_layout, active_video_page_size) == 0x4cu);
  static_assert(offsetof(bios_data_area_layout, active_video_page_offset) == 0x4eu);
  static_assert(offsetof(bios_data_area_layout, page_cursor_position) == 0x50u);
  static_assert(offsetof(bios_data_area_layout, cursor_shape) == 0x60u);
  static_assert(offsetof(bios_data_area_layout, active_video_page) == 0x62u);
  static_assert(offsetof(bios_data_area_layout, video_io_port) == 0x63u);
  static_assert(offsetof(bios_data_area_layout, video_mode_register) == 0x65u);
  static_assert(offsetof(bios_data_area_layout, color_palette) == 0x66u);
  static_assert(offsetof(bios_data_area_layout, adapter_rom_offset) == 0x67u);
  static_assert(offsetof(bios_data_area_layout, adapter_rom_segment) == 0x69u);
  static_assert(offsetof(bios_data_area_layout, last_interrupt_bit) == 0x6bu);
  static_assert(offsetof(bios_data_area_layout, interrupt_1a_counter) == 0x6cu);
  static_assert(offsetof(bios_data_area_layout, timer_24h_flag) == 0x70u);
  static_assert(offsetof(bios_data_area_layout, keyboard_ctrl_break_flag) == 0x71u);
  static_assert(offsetof(bios_data_area_layout, soft_reset_flag) == 0x72u);
  static_assert(offsetof(bios_data_area_layout, last_harddisk_operation_status) == 0x74u);
  static_assert(offsetof(bios_data_area_layout, number_of_harddisk_drives) == 0x75u);
  static_assert(offsetof(bios_data_area_layout, harddisk_control_byte) == 0x76u);
  static_assert(offsetof(bios_data_area_layout, harddisk_drive_port_offset) == 0x77u);
  static_assert(offsetof(bios_data_area_layout, lpt_port_timeout) == 0x78u);
  static_assert(offsetof(bios_data_area_layout, com_port_timeout) == 0x7cu);
  static_assert(offsetof(bios_data_area_layout, keyboard_buffer_begin) == 0x80u);
  static_assert(offsetof(bios_data_area_layout, keyboard_buffer_end) == 0x82u);
  static_assert(offsetof(bios_data_area_layout, number_of_text_rows_minus_one) == 0x84u);
  static_assert(offsetof(bios_data_area_layout, number_of_scanlines_per_character) == 0x85u);
  static_assert(offsetof(bios_data_area_layout, video_adapter_options) == 0x87u);
 
  static_assert(sizeof(bios_data_area_layout) == 0x100u);
  
  #pragma pack(pop)

  static inline auto bios_data_area () 
    -> volatile bios_data_area_layout& 
  { 
    return *std::launder((volatile bios_data_area_layout*)(0x400u)); 
  }

  static inline auto bda() 
    -> volatile bios_data_area_layout& 
  { 
    return bios_data_area(); 
  }
}