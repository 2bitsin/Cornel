#pragma once

#include <string_view>

struct script
{
  auto exec(std::string_view) -> script&;  
  auto status() const noexcept -> int;

  
};