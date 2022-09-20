#include <netboot32/call16.hpp>
#include <netboot32/netboot32.hpp>

void netboot32::call16_fun (call16_context_type& ctx, std::uint16_t cs, std::uint16_t ip)
{
  netboot32::finalize(false);
  x86arch::call16_fun(ctx, cs, ip);
  netboot32::initialize(false);
}

void netboot32::call16_int (call16_context_type& ctx, std::uint8_t num)
{
  netboot32::finalize(false);
  x86arch::call16_int(ctx, num);
  netboot32::initialize(false);
}