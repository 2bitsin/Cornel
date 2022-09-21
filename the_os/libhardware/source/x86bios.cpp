#include <hardware/x86bios.hpp>
#include <hardware/x86call16.hpp>
#include <hardware/x86call16_stack.hpp>

auto x86arch::bios_conventional_memory_size() -> std::uint32_t
{
  x86arch::call16_context ctx;
  x86arch::call16_stack stack{ctx, 0x400u};    
  return (x86arch::call16_invoke(ctx, 0x12) & 0xffffu) * 1024u;
}

auto x86arch::bios_acpi_memory_map_read(bios_acpi_memory_map_entry_t& entry, std::uint32_t last = 0u) -> std::uint32_t
{
  x86arch::call16_context ctx;
  x86arch::call16_stack stack{ctx, 0x400u};    
  auto entry_address = real_address::from_pointer(&entry);
  ctx.eax = 0x0000e820;
  ctx.edx = 0x534d4150;
  ctx.ecx = sizeof(bios_acpi_memory_map_entry_t);
  ctx.ebx = last;
  ctx.es  = entry_address.seg;
  ctx.edi = entry_address.off;
  x86arch::call16_invoke(ctx, 0x15);
  // TODO: finish this
  if (ctx.flags & 0x1u)
  { return 0u; }
}