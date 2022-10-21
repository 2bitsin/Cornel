#pragma once

#include <concepts>

namespace detail
{
  template <std::invocable F>
  struct defer_iml
  {
    defer_iml(F&& call) noexcept: m_call (std::forward<F>(call)) {}
   ~defer_iml() noexcept { m_call(); }

    auto release() noexcept -> F {
      return std::move(m_call);   
    }
  private:
    F m_call;
  };
}

#define CONCAT(a, b) CONCAT_INNER(a, b)
#define CONCAT_INNER(a, b) a ## b

#define defer(code) auto CONCAT(__Defer_, __COUNTER__) { ::detail::defer_iml {[&]{ code; }} }

