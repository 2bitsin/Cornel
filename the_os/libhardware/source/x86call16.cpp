#include <hardware/x86call16.hpp>
#include <utils/debug.hpp>
#include <call16_thunk/call16_thunk.hpp>

#include <cstddef>
#include <cstdint>
#include <memory>
#include <cstring>
#include <new>

using x86arch::call16_context_type;

using call16_function = void __attribute__((cdecl)) ();

void x86arch::call16_fun([[maybe_unused]] call16_context_type& ctx, [[maybe_unused]] std::uint16_t cs, [[maybe_unused]] std::uint16_t ip)
{
  using namespace x86arch::detail;
  decltype(auto) call16_thunk = (*(call16_thunk_layout*)call16_thunk_target);
  std::copy(call16_thunk_begin, call16_thunk_end, std::begin(call16_thunk.code));  
  std::construct_at(&call16_thunk.regs, std::move (ctx));
  call16_thunk.addr.cs = cs;
  call16_thunk.addr.ip = ip;
  ((call16_function*)&call16_thunk.code[0])();
  std::construct_at(&ctx, std::move (call16_thunk.regs));    
}

void x86arch::call16_int([[maybe_unused]] call16_context_type& ctx, [[maybe_unused]] std::uint8_t num)
{
  using namespace x86arch::detail;
  decltype(auto) call16_thunk = (*(call16_thunk_layout*)call16_thunk_target);
  std::copy(call16_thunk_begin, call16_thunk_end, std::begin(call16_thunk.code));  
  *std::launder((std::uint8_t*)(1u + call16_thunk.addr.ip + 0x10u*call16_thunk.addr.cs)) = num;
  std::construct_at(&call16_thunk.regs, std::move (ctx));    
  __debugbreak();
  ((call16_function*)&call16_thunk.code[0])();
  __debugbreak();
  std::construct_at(&ctx, std::move (call16_thunk.regs));
}
