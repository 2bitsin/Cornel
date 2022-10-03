#include <hardware/kbdctrl.hpp>
#include <hardware/x86asm.hpp>


void kbdctrl::wait_for_input_buffer_empty()
{
  while (x86arch::inb(kbdctrl::status_port) & kbdctrl::status_input_buffer_full_bit);
}

void kbdctrl::wait_for_output_buffer_not_empty()
{
  while (!(x86arch::inb(kbdctrl::status_port) & kbdctrl::status_output_buffer_full_bit));
}


void kbdctrl::send_command(std::uint8_t value, std::uint8_t wait)
{
  if (wait)
    kbdctrl::wait_for_input_buffer_empty();
  x86arch::outb(kbdctrl::command_port, value); 
}

void kbdctrl::send_data(std::uint8_t value, std::uint8_t wait)
{
  if (wait)
    kbdctrl::wait_for_input_buffer_empty();
  x86arch::outb(kbdctrl::data_port, value);
}

std::uint8_t kbdctrl::read_data(std::uint8_t wait)
{
  if (wait)
    kbdctrl::wait_for_output_buffer_not_empty();
  return x86arch::inb(kbdctrl::data_port);  
}

void kbdctrl::pulse_output_lines(std::uint8_t value)
{
  kbdctrl::send_command(kbdctrl::command_pulse_output_lines | ((~value) & 0xf), kbdctrl::wait);
}

void kbdctrl::hardware_reset()
{
  kbdctrl::pulse_output_lines(kbdctrl::line_reset);
}

void kbdctrl::send_output_port(std::uint8_t value)
{
  kbdctrl::send_command(kbdctrl::command_write_output_port, kbdctrl::wait);
  kbdctrl::send_data(value, kbdctrl::wait);
}

std::uint8_t kbdctrl::read_output_port()
{
  kbdctrl::send_command(kbdctrl::command_read_output_port, kbdctrl::wait);
  return kbdctrl::read_data(kbdctrl::wait);
}

void kbdctrl::disable_keyboard()
{
  kbdctrl::send_command(kbdctrl::command_disable_keyboard, kbdctrl::wait);
}
void kbdctrl::enable_keyboard()
{
  kbdctrl::send_command(kbdctrl::command_enable_keyboard, kbdctrl::wait);
}
