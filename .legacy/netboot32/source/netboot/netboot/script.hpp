#pragma once

#include <string_view>

struct script
{
  struct execute_context
  {
    std::size_t line_number;
  };

  auto execute(std::string_view text)  -> script&;  
  auto status() const  -> int;
protected:
  auto execute_line(std::string_view line)  -> script&;
  auto report_error(std::string_view error)  -> script&;
private:

  int m_status { 0 };
};