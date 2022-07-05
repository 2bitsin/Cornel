#include "kbdctrl.h"
#include "x86assembly.h"
#include "types.h"

#define KBC_STATUS_PORT                     0x64
#define KBC_COMMAND_PORT                    0x64
#define KBC_DATA_PORT                       0x60

#define KBC_STATUS_OUTPUT_BUFFER_FULL_BIT   0x01
#define KBC_STATUS_INPUT_BUFFER_FULL_BIT    0x02

#define KBC_COMMAND_PULSE_OUTPUT_LINES      0xF0
#define KBC_LINE_RESET                      0x01

void KBC_send_command(uint8_t value)
{
  while (x86_inb(KBC_STATUS_PORT) & KBC_STATUS_INPUT_BUFFER_FULL_BIT);
  x86_outb(KBC_COMMAND_PORT, value); 
}

void KBC_pulse_output_lines(uint8_t value)
{
  KBC_send_command(KBC_COMMAND_PULSE_OUTPUT_LINES | ((~value) & 0xf));
}

void KBC_hardware_reset()
{
  KBC_pulse_output_lines(KBC_LINE_RESET);
}
