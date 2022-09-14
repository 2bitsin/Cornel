#pragma once

struct memory
{
  static auto initialize(bool first_time) -> void;
  static auto finalize(bool last_time) -> void;
};