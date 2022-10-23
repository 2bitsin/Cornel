#include <cstddef>
#include <cstdint>
#include <memory>
#include <new>

#include <utils/debug.hpp>
#include <memory/buffer.hpp>
#include <call16_thunk/call16_thunk.hpp>
#include <hardware/x86call16.hpp>
#include <hardware/x86assembly.hpp>
#include <hardware/pic8259.hpp>

using call16_function = void __attribute__((cdecl)) ();

template <typename Target>
requires (std::is_same_v<Target, x86arch::call16_address> || std::is_same_v<Target, std::uint8_t>)
static inline std::uint32_t call16_invoke(x86arch::call16_context& ctx, Target const& target)
{
  using namespace x86arch::detail;
  std::uint16_t save_mask;
  std::span<std::uint64_t> save_idtr;

  static constexpr auto reserved_size = sizeof(call16_thunk_layout) + 256u;
  if (call16_thunk_target + reserved_size > x86arch::stack_pointer())
  {
    std::__throw_runtime_error("Call16_invoke will stomp on stack");
  }

  decltype(auto) call16_thunk = (*(call16_thunk_layout*)call16_thunk_target);
  std::copy(call16_thunk_begin, call16_thunk_end, std::begin(call16_thunk.code));  
  std::construct_at(&call16_thunk.regs, std::move (ctx));

  if constexpr(!std::is_same_v<Target, x86arch::call16_address>) 
  { call16_thunk.addr.as<std::uint8_t*>()[1u] = target; }
  else
  { std::construct_at(&call16_thunk.addr, std::move (target)); }

  {  
    x86arch::disable_interrupts _;
    save_mask = pic8259::read_mask();    
    pic8259::write_mask(ctx.irq_mask);
    pic8259::switch_to_real_mode();
    save_idtr = x86arch::sidt();
    x86arch::lidt (x86arch::Xdtr_t{ .limit = 0x400, .base = nullptr });

    //__debugbreak();
    ((call16_function*)&call16_thunk.code[0])();

    x86arch::lidt (save_idtr);
    pic8259::switch_to_prot_mode();
    pic8259::write_mask (save_mask);
  }
  std::construct_at(&ctx, std::move (call16_thunk.regs));
  return ctx.eax;
}

std::uint32_t x86arch::call16_invoke (call16_context& ctx, call16_address const& target)
{ return ::call16_invoke(ctx, target); }

std::uint32_t x86arch::call16_invoke (call16_context& ctx, std::uint8_t target)
{ return ::call16_invoke(ctx, target); }
