#pragma once

#include <string_view>

namespace script
{
	
	using namespace std::string_view_literals;
	using namespace std::string_literals;
	
	struct script
	{			
		static inline constexpr auto G_white_space = " \t\r\n"sv;
		
		static inline constexpr auto is_white_space(char char_v) noexcept -> bool
		{
			return G_white_space.find(char_v) != std::string_view::npos;
		}
		
		static inline constexpr auto trim_white_space(std::string_view& string_v)
		{
		  while(!string_v.empty() && is_white_space(string_v.front()))
		    string_v.remove_prefix(1);
		  while(!string_v.empty() && is_white_space(string_v.back()))
		    string_v.remove_suffix(1);  
		}
		
		static inline constexpr auto trim_comments(std::string_view& string_v)
		{
		  if(string_v.empty())
		    return;
		  auto pos = string_v.find('#');
		  if(pos != std::string_view::npos)
		    string_v.remove_suffix(string_v.size() - pos);
		}
			
		auto execute(auto&& executor_v, std::string_view text_v) noexcept -> script&
		{
			std::string_view line;
			while (!text_v.empty())
		  {
				executor_v.begin();						
				if (auto pos = text_v.find('\n'); pos != text_v.npos)
				{
					line = text_v.substr(0u, pos);
					text_v = text_v.substr(pos + 1u);
				}
				else {
					line = std::exchange(text_v, ""sv);
				}
								
				// Trim comments & white space
		    trim_comments(line);
		    trim_white_space(line);
					
				// If	the line is empty, skip it
				if (line.empty())
					continue;
				
				// Otherwise tokenize and execute the line
				tokenize(std::forward<decltype(executor_v)>(executor_v), line);
				executor_v.end();
				
		  }
		  return *this;
		}

	protected:				
	  auto tokenize(auto&& executor_v, std::string_view line_v) noexcept -> script&
		{  	  
		  if (line_v.empty())
		    return *this;
		  
		  while (!line_v.empty())
		  {
				// Handle non quoted cases
		    if (line_v.front() != '"') 
		    {				
					const auto is_white_space_v = is_white_space(line_v.front());
					const auto pos = std::min(!is_white_space_v
						? line_v.find_first_of(G_white_space) 
						: line_v.find_first_not_of(G_white_space),
						line_v.size());
					if (!is_white_space_v)
						executor_v.emit(line_v.substr(0u, pos));
					line_v.remove_prefix(pos);
					continue;
		    }
				
				// Handle quoted cases
				for(size_t pos=0u;;) 
				{
					pos = line_v.find('"', pos + 1u);
					if (pos == line_v.npos) {
						executor_v.error("Can't find matching \" character.");
						return *this;
					}				
					if (pos > 1u && line_v[pos - 1u] == '\\') 
						continue;				
					executor_v.emit(line_v.substr(1u, pos - 1u));
					line_v.remove_prefix(pos + 1u);
					break;
				}
			}
			return *this;
		}		  
	};	
}
