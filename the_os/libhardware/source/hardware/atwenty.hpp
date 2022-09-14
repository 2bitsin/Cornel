#pragma once

#include <cstdint>
#include <cstddef>

struct atwenty
{
  static auto is_enabled() -> bool;
  static auto try_enable() -> bool;

  static auto initialize() -> void;
  static auto finalize() -> void;
};


