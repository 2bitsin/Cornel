#include <hardware/ibm/pic8259.hpp>
#include <hardware/x86/assembly.hpp>

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
  using namespace x86arch;
  return (inb(PIC8259B_PORT+1u) * 0x100u) | inb(PIC8259A_PORT+1u) ;
}

auto pic8259::write_mask(std::uint16_t m) -> void
{
  using namespace x86arch;
  std::uint8_t lower = m & 0xffu;
  std::uint8_t upper = m >> 8u;
  if (lower) outb(PIC8259A_PORT+1u, lower);
  if (upper) outb(PIC8259B_PORT+1u, upper);
}

void pic8259::configure (std::uint8_t offset_a, std::uint8_t offset_b)
{
  using namespace x86arch;

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

void pic8259::end_of_interrupt (std::uint8_t irq)
{
  using namespace x86arch;
  if (irq >= 8) 
    outb (PIC8259B_PORT, 0x20);
  outb (PIC8259A_PORT, 0x20);
}

void pic8259::switch_to_prot_mode ()
{
  pic8259::configure(0x20u, 0x28u);
}

void pic8259::switch_to_real_mode ()
{  
  pic8259::configure(0x08u, 0x70u);
}



/*

1000111110111000

15 - 1   - Secondary ATA
14 - 0   - Primary ATA
13 - 0   - FPU
12 - 0   - PS/2 Mouse
11 - 1   - ******
10 - 1   - ******
09 - 1   - ******
08 - 1   - CMOS / RTC
07 - 1   - LPT1
06 - 0   - Floppy
05 - 1   - LPT2
04 - 1   - COM1/COM3
03 - 1   - COM2/COM4
02 - 0   - ******
01 - 0   - PS/2 Keyboard
00 - 0   - PIT
*/
