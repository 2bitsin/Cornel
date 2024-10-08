#include <hardware/x86/address16.hpp>
#include <hardware/x86/bios.hpp>
#include <hardware/x86/call16.hpp>
#include <hardware/x86/call16_stack.hpp>
#include <hardware/x86/assembly.hpp>

#include <system_error>
#include <memory_resource>

auto x86arch::bios_conventional_memory_size() -> std::uint32_t
{
  x86arch::call16_context ctx;
  x86arch::call16_stack stack{ ctx, 0x400u };    
  return (x86arch::call16_invoke(ctx, 0x12) & 0xffffu) * 1024u;
}

auto x86arch::bios_acpi_memory_map_read(bios_acpi_memory_map_entry_t& entry, std::uint32_t& o_length, std::uint32_t& o_offset, std::uint32_t i_offset) -> std::errc
{
  x86arch::call16_context ctx;
  x86arch::call16_stack stack{ ctx, 0x400u };    
  auto [off, seg] = address16::from(&entry);
  ctx.eax = 0x0000e820;
  ctx.edx = 0x534d4150;
  ctx.ecx = sizeof(bios_acpi_memory_map_entry_t);
  ctx.ebx = i_offset;
  ctx.es  = seg;
  ctx.edi = off;  
  x86arch::call16_invoke(ctx, 0x15);  
  if ((ctx.flags & x86arch::flags::carry) || (ctx.eax != 0x534d4150)) { 
    return std::errc::not_supported;    
  } 
  o_length = ctx.ecx;
  o_offset = ctx.ebx;
  return std::errc{};
}

auto x86arch::read_bios_data_area(std::uint8_t offset_i, void* value_o, std::size_t size_i) -> void
{
  std::memcpy(value_o, (const void*)(0x400u + offset_i), size_i);
}