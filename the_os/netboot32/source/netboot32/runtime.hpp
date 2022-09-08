#pragma once


struct runtime
{
  static auto initialize() -> void;
  static auto finalize() -> void;
  static auto panick() -> void;
};