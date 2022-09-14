#include <hardware/rtccmos.hpp>
#include <hardware/assembly.hpp>

static inline constexpr auto NMI_DISABLE_BIT = 0x80u;

void rtccmos::nmi_disable() noexcept
{
  using namespace assembly;
  outb (0x70, inb(0x70) | NMI_DISABLE_BIT);
}

void rtccmos::nmi_enable() noexcept
{
  using namespace assembly;
  outb (0x70, inb(0x70) & ~NMI_DISABLE_BIT);
}

auto rtccmos::read_byte(uint8_t addr) noexcept -> std::uint8_t
{
  using namespace assembly;
  std::uint8_t nmi_status, value;  
  nmi_status = inb(0x70) & NMI_DISABLE_BIT;
  cli();
  outb(0x70, addr | NMI_DISABLE_BIT);
  value = inb(0x71);
  outb(0x70, nmi_status);
  return value;
}

