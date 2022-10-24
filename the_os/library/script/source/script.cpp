#include <script/script.hpp>

#include <string>
#include <iostream>

using namespace std::string_view_literals;
using namespace std::string_literals;

static constexpr auto G_white_space = " \t\r\n"sv;

static constexpr auto is_white_space(char char_v) noexcept -> bool
{
	return G_white_space.find(char_v) != std::string_view::npos;
}

static constexpr auto trim_white_space(std::string_view& string_v)
{
  while(!string_v.empty() && is_white_space(string_v.front()))
    string_v.remove_prefix(1);
  while(!string_v.empty() && is_white_space(string_v.back()))
    string_v.remove_suffix(1);  
}

static constexpr auto trim_comments(std::string_view& string_v)
{
  if(string_v.empty())
    return;
  auto pos = string_v.find('#');
  if(pos != std::string_view::npos)
    string_v.remove_suffix(string_v.size() - pos);
}

auto script::execute(execute_context& context_v, std::string_view text_v) noexcept -> script&
{
	std::vector<std::string_view> tokens_v;
	while (!text_v.empty())
  {
		context_v.next_line();
		
		auto pos = text_v.find('\n');
		if (pos == text_v.npos)
			pos = text_v.size();
		
		// Slice off the line
    auto line = text_v.substr(0u, pos);
    text_v = text_v.substr(pos + 1u);
						
		// Trim comments & white space
    trim_comments(line);
    trim_white_space(line);
			
		// If	the line is empty, skip it
		if (line.empty())
			continue;
		
		// Otherwise tokenize and execute the line
		tokens_v.clear();
    breakup_line(context_v, line, tokens_v);
		//if (!tokens_v.empty())
		//	execute_line(context_v, tokens_v);
			
  }
  return *this;
}

auto script::status() const noexcept -> int
{
	return m_status;
}

auto script::breakup_line(execute_context& context_v, std::string_view line_v, std::vector<std::string_view>& tokens_v) noexcept -> script&
{  
	tokens_v.reserve(32);
  std::size_t pos { 0u };
  
  if (line_v.empty())
    return *this;
  
  while (!line_v.empty())
  {
    if (line_v.front() == '"') 
    {
			std::size_t pos { 0u };
			while(true)
			{
				pos = line_v.find('"', pos + 1u);
				if (pos == std::string_view::npos) 
				{
					report_error(context_v, "Can't find matching \" character.");
					return *this;
				}
				
				if (pos > 1u && line_v[pos - 1u] == '\\') 
					continue;
				
				tokens_v.emplace_back(line_v.substr(1u, pos - 1u));
				line_v.remove_prefix(pos + 1u);
				break;
			}
    }
    else if (!is_white_space(line_v.front()))
    {      
			const auto pos = line_v.find_first_of(G_white_space);
			if (pos == std::string_view::npos)
			{
				tokens_v.emplace_back(line_v);
				line_v.remove_prefix(line_v.size());
			}
			else
			{
				tokens_v.emplace_back(line_v.substr(0u, pos));
				line_v.remove_prefix(pos);
			}
	  }
		else
		{
			const auto pos = line_v.find_first_not_of(G_white_space);
			if (pos == std::string_view::npos)
			{
				line_v.remove_prefix(line_v.size());
			}
			else
			{
				line_v.remove_prefix(pos);
			}
		}
	}
	return *this;
}

auto script::report_error(execute_context const& context_v, std::string_view msg) noexcept -> script&
{
	throw std::runtime_error(std::string(msg));
	return *this;
}