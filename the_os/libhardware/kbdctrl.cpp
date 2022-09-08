#include <hardware/kbdctrl.hpp>
#include <hardware/assembly.hpp>


void kbdctrl::wait_for_input_buffer_empty()
{
  while (assembly::inb(kbdctrl::STATUS_PORT) & kbdctrl::STATUS_INPUT_BUFFER_FULL_BIT);
}

void kbdctrl::wait_for_output_buffer_not_empty()
{
  while (!(assembly::inb(kbdctrl::STATUS_PORT) & kbdctrl::STATUS_OUTPUT_BUFFER_FULL_BIT));
}


void kbdctrl::send_command(std::uint8_t value, std::uint8_t wait)
{
  if (wait)
    kbdctrl::wait_for_input_buffer_empty();
  assembly::outb(kbdctrl::COMMAND_PORT, value); 
}

void kbdctrl::send_data(std::uint8_t value, std::uint8_t wait)
{
  if (wait)
    kbdctrl::wait_for_input_buffer_empty();
  assembly::outb(kbdctrl::DATA_PORT, value);
}

std::uint8_t kbdctrl::read_data(std::uint8_t wait)
{
  if (wait)
    kbdctrl::wait_for_output_buffer_not_empty();
  return assembly::inb(kbdctrl::DATA_PORT);  
}

void kbdctrl::pulse_output_lines(std::uint8_t value)
{
  kbdctrl::send_command(kbdctrl::COMMAND_PULSE_OUTPUT_LINES | ((~value) & 0xf), kbdctrl::WAIT);
}

void kbdctrl::hardware_reset()
{
  kbdctrl::pulse_output_lines(kbdctrl::LINE_RESET);
}

void kbdctrl::send_output_port(std::uint8_t value)
{
  kbdctrl::send_command(kbdctrl::COMMAND_WRITE_OUTPUT_PORT, kbdctrl::WAIT);
  kbdctrl::send_data(value, kbdctrl::WAIT);
}

std::uint8_t kbdctrl::read_output_port()
{
  kbdctrl::send_command(kbdctrl::COMMAND_READ_OUTPUT_PORT, kbdctrl::WAIT);
  return kbdctrl::read_data(kbdctrl::WAIT);
}

void kbdctrl::disable_keyboard()
{
  kbdctrl::send_command(kbdctrl::COMMAND_DISABLE_KEYBOARD, kbdctrl::WAIT);
}
void kbdctrl::enable_keyboard()
{
  kbdctrl::send_command(kbdctrl::COMMAND_ENABLE_KEYBOARD, kbdctrl::WAIT);
}
