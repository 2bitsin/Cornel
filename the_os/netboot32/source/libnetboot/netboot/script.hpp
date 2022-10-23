#pragma once

#include <string_view>

struct script
{
  struct execute_context
  {
    std::size_t line_number;
  };

  auto execute(std::string_view text) noexcept -> script&;  
  auto status() const noexcept -> int;
protected:
  auto execute_line(std::string_view line) noexcept -> script&;
  auto report_error(std::string_view error) noexcept -> script&;
private:

  int m_status { 0 };
};