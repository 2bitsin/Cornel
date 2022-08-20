#ifndef __KBDCTRL_H__
#define __KBDCTRL_H__

#include "types.h"

#define KBC_STATUS_OUTPUT_BUFFER_FULL_BIT   0x01
#define KBC_STATUS_INPUT_BUFFER_FULL_BIT    0x02

#define KBC_COMMAND_DISABLE_KEYBOARD        0xAD
#define KBC_COMMAND_ENABLE_KEYBOARD         0xAE
#define KBC_COMMAND_READ_INPUT_PORT         0xC0
#define KBC_COMMAND_READ_OUTPUT_PORT        0xD0
#define KBC_COMMAND_WRITE_OUTPUT_PORT       0xD1
#define KBC_COMMAND_READ_CONFIGURATION      0x20
#define KBC_COMMAND_WRITE_CONFIGURATION     0x60
#define KBC_COMMAND_SELF_TEST               0xAA
#define KBC_COMMAND_INTERFACE_TEST          0xAB
#define KBC_COMMAND_READ_TEST_INPUTS        0xE0
#define KBC_COMMAND_PULSE_OUTPUT_LINES      0xF0

#define KBC_OUTPUT_PORT_A20_ENABLE_BIT      0x02


#define KBC_LINE_RESET                      0x01
#define KBC_WAIT                            0x01
#define KBC_DONT_WAIT                       0x00


void KBC_hardware_reset();
void KBC_send_command(uint8_t value, uint8_t wait);
byte KBC_read_data(uint8_t wait);
void KBC_send_data(uint8_t value, uint8_t wait);
void KBC_pulse_output_lines(uint8_t value);

void KBC_disable_keyboard();
void KBC_enable_keyboard();
byte KBC_read_output_port();
void KBC_send_output_port(byte value);

void KBC_wait_for_output_buffer_not_empty();
void KBC_wait_for_input_buffer_empty();

#endif
