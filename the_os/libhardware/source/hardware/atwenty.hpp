#pragma once

#include <cstdint>
#include <cstddef>

struct atwenty
{
  static auto is_enabled() -> bool;
  static auto try_enable() -> bool;

  static auto initialize(bool first_time) -> void;
  static auto finalize(bool last_time) -> void;
};


