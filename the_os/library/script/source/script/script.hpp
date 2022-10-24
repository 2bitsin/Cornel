#pragma once

#include <string_view>
#include <vector>

struct script
{
  struct execute_context
  {		
		virtual ~execute_context() = default;		
		virtual void next_line() { ++m_line; };
		
		virtual auto line() const noexcept -> std::size_t 
		{ return m_line; };
	
	private:
    std::size_t m_line { 0u };
  };

  auto execute(execute_context& context_v, std::string_view text_v) noexcept -> script&;  
  auto status() const noexcept -> int;
protected:
  auto breakup_line(execute_context& context_v, std::string_view line_v, std::vector<std::string_view>& tokens_v) noexcept -> script&;
  auto report_error(execute_context const& context_v, std::string_view error_v) noexcept -> script&;
private:

  int m_status { 0 };
};