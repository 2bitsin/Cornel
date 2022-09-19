#include <netboot32/call16.hpp>
#include <memory>


void x86arch::call_fun16(call_context_type& ctx, std::uint32_t eip, std::uint16_t cs)
{
  auto staging_area = std::make_unique<std::uint8_t[]>(0x1000);


}

void x86arch::call_int16(call_context_type& ctx, std::uint8_t num)
{

}
