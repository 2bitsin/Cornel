#include <cstdlib>

#include <hardware/atwenty.hpp>
#include <hardware/assembly.hpp>
#include <hardware/kbdctrl.hpp>
#include <hardware/pic8259.hpp>
#include <hardware/console.hpp>

#include <misc/utilities_var.hpp>
#include <textio/simple.hpp>

static inline const constexpr std::uint8_t FAST_GATE_A20_ENABLE_BIT = 0x02u;
static inline const constexpr std::uint8_t FAST_GATE_RESET_BIT = 0x01u;

static inline void atwenty_port_ee_enable()
{
  assembly::inb(0xee);
}

static inline void atwenty_fast_enable()
{
  uint8_t state = assembly::inb(0x92);
  if (!(state & FAST_GATE_A20_ENABLE_BIT)) 
  {
    state &= ~FAST_GATE_RESET_BIT;    
    state |= FAST_GATE_A20_ENABLE_BIT;
    assembly::outb(0x92, state);
  }
}

static inline void atwenty_kbdctl_enable()
{
  // TODO: disable interrupts
  auto save_mask = pic8259::read_mask();
  pic8259::write_mask(0xff);  
  std::uint8_t status; 
  kbdctrl::disable_keyboard();
  status = kbdctrl::read_output_port();
  status |= kbdctrl::OUTPUT_PORT_A20_ENABLE_BIT;
  kbdctrl::send_output_port(status); 
  kbdctrl::enable_keyboard();
  pic8259::write_mask(save_mask);
  // TODO: restore interrupt flag state
}

/*
uint16_t A20_enable_bios_15h();
#pragma aux A20_enable_bios_15h = \
  "stc"             \
  "mov ax, 0x2401"  \
  "int 0x15"        \
  "mov al, ah"      \
  "lahf"            \
  "and ah, 1"       \
  value [ax]        \
  */

auto atwenty::is_enabled() -> bool
{
  using Q = std::uint8_t;
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Warray-bounds"  
  auto& lower = *(volatile Q*)(0x000500u);
  auto& upper = *(volatile Q*)(0x100500u);
  upper = ~lower;
  return upper == (Q)~lower;
 #pragma GCC diagnostic pop
}

auto atwenty::try_enable() -> bool
{
  if (is_enabled()) 
    return true;  

  atwenty_port_ee_enable();

  for (auto i = 0u; i < 10000u && !is_enabled(); ++i);
  if (is_enabled())
    return true;

  atwenty_fast_enable();

  for (auto i = 0u; i < 10000u && !is_enabled(); ++i);
  if (is_enabled())
    return true;

  atwenty_kbdctl_enable();

  for (auto i = 0u; i < 10000u && !is_enabled(); ++i);
  if (is_enabled())
    return true;
  
  // TODO : Add bios enable fallback
  return false;
}

auto atwenty::initialize(bool first_time) -> void
{
  using namespace textio::simple;
  if (!first_time)
    return;
  if (!try_enable()) 
  {
    console::writeln("#001 - Failed to enable address line 20.");
    std::abort();
  }
}

auto atwenty::finalize([[maybe_unused]] bool last_time) -> void
{
}
