#pragma once

#include <cstddef>
#include <cstdint>

struct rtc
{
  static void nmi_enable() noexcept;
  static void nmi_disable() noexcept;
  static auto read_byte(std::uint8_t addr) noexcept -> std::uint8_t;

  static inline constexpr auto ADDR_SECONDS_REG           = 0x00u;
  static inline constexpr auto ADDR_ALARM_SECONDS_REG     = 0x01u;
  static inline constexpr auto ADDR_MINUTES_REG           = 0x02u;
  static inline constexpr auto ADDR_ALARM_MINUTES_REG     = 0x03u;
  static inline constexpr auto ADDR_HOURS_REG             = 0x04u;
  static inline constexpr auto ADDR_ALARM_HOURS_REG       = 0x05u;
  static inline constexpr auto ADDR_WEEKDAY_REG           = 0x06u;
  static inline constexpr auto ADDR_DAY_REG               = 0x07u;
  static inline constexpr auto ADDR_MONTH_REG             = 0x08u;
  static inline constexpr auto ADDR_YEAR_REG              = 0x09u;
  static inline constexpr auto ADDR_STATUS_A_REG          = 0x0Au;
  static inline constexpr auto ADDR_STATUS_B_REG          = 0x0Bu;
  static inline constexpr auto ADDR_STATUS_C_REG          = 0x0Cu;
  static inline constexpr auto ADDR_STATUS_D_REG          = 0x0Du;
  static inline constexpr auto ADDR_DIAG_STATUS_REG       = 0x0Eu;
  static inline constexpr auto ADDR_SHUTDOWN_STATUS_REG   = 0x0Fu;
  static inline constexpr auto ADDR_FLOPPY_DISK_REG       = 0x10u;
  static inline constexpr auto ADDR_SYS_CONFIG_REG        = 0x11u;
  static inline constexpr auto ADDR_HARD_DISK_REG         = 0x12u;
  static inline constexpr auto ADDR_TYPEMATIC_CONFIG_REG  = 0x13u;
  static inline constexpr auto ADDR_EQUIPMENT_REG         = 0x14u;
  static inline constexpr auto ADDR_BASE_MEM_LSB_REG      = 0x15u;
  static inline constexpr auto ADDR_BASE_MEM_MSB_REG      = 0x16u;
  static inline constexpr auto ADDR_EXT_MEM_LSB_REG       = 0x17u;
  static inline constexpr auto ADDR_EXT_MEM_MSB_REG       = 0x18u;
  static inline constexpr auto ADDR_CENTURY_REG           = 0x32u;
};

