#ifndef __RTCCMOS_H__
#define __RTCCMOS_H__

#include "types.h"

void RTC_nmi_disable();
void RTC_nmi_enable();

uint8_t CMOS_read_byte(uint8_t addr);

#define CMOS_SECONDS_REG          0x00
#define CMOS_ALARM_SECONDS_REG    0x01
#define CMOS_MINUTES_REG          0x02
#define CMOS_ALARM_MINUTES_REG    0x03
#define CMOS_HOURS_REG            0x04
#define CMOS_ALARM_HOURS_REG      0x05
#define CMOS_WEEKDAY_REG          0x06
#define CMOS_DAY_REG              0x07
#define CMOS_MONTH_REG            0x08
#define CMOS_YEAR_REG             0x09
#define CMOS_STATUS_A_REG         0x0A
#define CMOS_STATUS_B_REG         0x0B
#define CMOS_STATUS_C_REG         0x0C
#define CMOS_STATUS_D_REG         0x0D
#define CMOS_DIAG_STATUS_REG      0x0E
#define CMOS_SHUTDOWN_STATUS_REG  0x0F
#define CMOS_FLOPPY_DISK_REG      0x10
#define CMOS_SYS_CONFIG_REG       0x11
#define CMOS_HARD_DISK_REG        0x12
#define CMOS_TYPEMATIC_CONFIG_REG 0x13
#define CMOS_EQUIPMENT_REG        0x14
#define CMOS_BASE_MEM_LSB_REG     0x15
#define CMOS_BASE_MEM_MSB_REG     0x16
#define CMOS_EXT_MEM_LSB_REG      0x17
#define CMOS_EXT_MEM_MSB_REG      0x18

#define CMOS_CENTURY_REG          0x32

#endif

