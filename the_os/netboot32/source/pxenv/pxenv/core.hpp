#pragma once

#include <cstddef>
#include <cstdint>
#include <span>

struct PXENVplus;
struct bangPXE;

#include <pxenv/status.hpp>

namespace pxenv
{
  auto initialize       (bool first_time) -> void;
  auto finalize         (bool last_time) -> void;
  auto call             (void* params, std::uint16_t opcode) -> pxenv_status;
};
