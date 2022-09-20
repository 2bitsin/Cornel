#include <cstddef>
#include <cstdint>
#include <memory>
#include <cstring>
#include <new>

#include <utils/debug.hpp>

#include <call16_thunk/call16_thunk.hpp>

#include <hardware/x86call16.hpp>
#include <hardware/pic8259.hpp>
#include <hardware/x86arch.hpp>

using call16_function = void __attribute__((cdecl)) ();

template <typename Target>
requires (std::is_same_v<Target, x86arch::call16_address> || std::is_same_v<Target, std::uint8_t>)
static inline void call16_invoke(x86arch::call16_context& ctx, Target const& target)
{
  using namespace x86arch::detail;
  decltype(auto) call16_thunk = (*(call16_thunk_layout*)call16_thunk_target);
  std::copy(call16_thunk_begin, call16_thunk_end, std::begin(call16_thunk.code));  
  std::construct_at(&call16_thunk.regs, std::move (ctx));

  if constexpr(!std::is_same_v<Target, x86arch::call16_address>) 
  { call16_thunk.addr.to_pointer<std::uint8_t>()[1u] = target; }
  else
  { std::construct_at(&call16_thunk.addr, std::move (target)); }

  {  
    x86arch::interrupt_guard _;

    auto save_mask = pic8259::read_mask();
    pic8259::switch_to_real_mode();
    auto save_idtr = x86arch::sidt();
    x86arch::lidt (x86arch::Xdtr_t{ .limit = 0x400, .base = nullptr });

    ((call16_function*)&call16_thunk.code[0])();

    x86arch::lidt (save_idtr);
    pic8259::switch_to_prot_mode();
    pic8259::write_mask (save_mask);
  }
  std::construct_at(&ctx, std::move (call16_thunk.regs));
}

void x86arch::call16_invoke (call16_context& ctx, call16_address const& target)
{ ::call16_invoke(ctx, target); }

void x86arch::call16_invoke (call16_context& ctx, std::uint8_t target)
{ ::call16_invoke(ctx, target); }


void x86arch::call16_stack_allocate(call16_context& ctx, std::uint16_t size)
{  
  auto stack_address = x86arch::real_address::from_pointer(new std::uint8_t[size]);  
  __debug_assert(stack_address.off == 0u);
  ctx.esp = stack_address.off + size;
  ctx.ss = stack_address.seg;
}

void x86arch::call16_stack_deallocate(call16_context& ctx)
{
  delete [] real_address{ .off = 0u, .seg = ctx.ss }.to_pointer<std::uint8_t>();
}