#pragma once

#include <cstdint>
#include <cstddef>

//std::int16_t A20_init();
//std::int16_t A20_check();

struct atwenty
{
  static auto is_enabled() -> bool;
  static auto try_enable() -> bool;
};


