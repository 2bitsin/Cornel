#include <hardware/pic8259.hpp>
#include <hardware/assembly.hpp>

static inline constexpr std::uint16_t PIC8259A_PORT = 0x20;
static inline constexpr std::uint16_t PIC8259B_PORT = 0xA0;

void pic8259::mask_or(std::uint16_t m) 
{
  pic8259::write_mask (pic8259::read_mask() | m);
}
 
void pic8259::mask_and(std::uint16_t m) 
{
  pic8259::write_mask (pic8259::read_mask() & m);  
}

auto pic8259::read_mask() -> std::uint16_t
{
  using namespace assembly;
  return (inb(PIC8259B_PORT+1u) * 0x100u) | inb(PIC8259A_PORT+1u) ;
}

auto pic8259::write_mask(std::uint16_t m) -> void
{
  using namespace assembly;
  std::uint8_t lower = m & 0xffu;
  std::uint8_t upper = m >> 8u;
  if (lower) outb(PIC8259A_PORT+1u, lower);
  if (upper) outb(PIC8259B_PORT+1u, upper);
}

void pic8259::configure (std::uint8_t offset_a, std::uint8_t offset_b)
{
  using namespace assembly;

  // Save masks
  auto save_mask = pic8259::read_mask();

  // Initialize PICs
  outb (PIC8259A_PORT+0u, 0x11) ;     // COMMAND: initialize , ICW4 needed
  outb (PIC8259B_PORT+0u, 0x11) ;     // COMMAND: initialize , ICW4 needed
  outb (PIC8259A_PORT+1u, offset_a) ; // ICW2: Master PIC vector offset
  outb (PIC8259B_PORT+1u, offset_b) ; // ICW2: Slave PIC vector offset
  outb (PIC8259A_PORT+1u, 0x4) ;      // ICW3: tell master PIC that there is a slave PIC at IRQ2 (0000 0100)
  outb (PIC8259B_PORT+1u, 0x2) ;      // ICW3: tell slave PIC its cascade identity (0000 0010)
  outb (PIC8259A_PORT+1u, 0x1) ;      // ICW4: 8086 mode
  outb (PIC8259B_PORT+1u, 0x1) ;      // ICW4: 8086 mode

  // Restore masks
  pic8259::write_mask(save_mask);
}