#include "kbdctrl.h"
#include "x86asm.h"
#include "types.h"

#define KBC_STATUS_PORT                     0x64
#define KBC_COMMAND_PORT                    0x64
#define KBC_DATA_PORT                       0x60


void KBC_wait_for_input_buffer_empty()
{
  while (x86_inb(KBC_STATUS_PORT) & KBC_STATUS_INPUT_BUFFER_FULL_BIT);
}

void KBC_wait_for_output_buffer_not_empty()
{
  while (!(x86_inb(KBC_STATUS_PORT) & KBC_STATUS_OUTPUT_BUFFER_FULL_BIT));
}


void KBC_send_command(uint8_t value, uint8_t wait)
{
  if (wait)
    KBC_wait_for_input_buffer_empty();
  x86_outb(KBC_COMMAND_PORT, value); 
}

void KBC_send_data(uint8_t value, uint8_t wait)
{
  if (wait)
    KBC_wait_for_input_buffer_empty();
  x86_outb(KBC_DATA_PORT, value);
}

byte KBC_read_data(uint8_t wait)
{
  if (wait)
    KBC_wait_for_output_buffer_not_empty();
  return x86_inb(KBC_DATA_PORT);  
}

void KBC_pulse_output_lines(uint8_t value)
{
  KBC_send_command(KBC_COMMAND_PULSE_OUTPUT_LINES | ((~value) & 0xf), KBC_WAIT);
}

void KBC_hardware_reset()
{
  KBC_pulse_output_lines(KBC_LINE_RESET);
}

void KBC_send_output_port(byte value)
{
  KBC_send_command(KBC_COMMAND_WRITE_OUTPUT_PORT, KBC_WAIT);
  KBC_send_data(value, KBC_WAIT);
}

byte KBC_read_output_port()
{
  KBC_send_command(KBC_COMMAND_READ_OUTPUT_PORT, KBC_WAIT);
  return KBC_read_data(KBC_WAIT);
}

void KBC_disable_keyboard()
{
  KBC_send_command(KBC_COMMAND_DISABLE_KEYBOARD, KBC_WAIT);
}
void KBC_enable_keyboard()
{
  KBC_send_command(KBC_COMMAND_ENABLE_KEYBOARD, KBC_WAIT);
}
