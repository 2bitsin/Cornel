#include "script.hpp"

static constexpr auto is_white_space(char char_v) noexcept -> bool
{
  switch(char_v)
  {
  case ' ':
  case '\t':
  case '\r':
  case '\n':
    return true;
  default:
    return false;
  }
  return false;
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

strcipt::execute(std::string_view text) noexcept -> script&
{
  while(!text.empty())
  {
    for(auto i = 0u; i < text.size(); ++i)
    {
      if (text[i] != '\n')
        continue;
      auto line = text.substr(0u, i);
      text = text.substr(i + 1u);
      trim_white_space(line);
      trim_comments(line);
      execute_line(line);
      break;
    }
  }
  return *this;
}

script::execute_line(std::string_view line)
{
  std::vector<std::string_view> tokens;
  std::size_t pos { 0u };
  
  if (line.empty())
    return *this;
  
  while (!line.empty())
  {
    if (line.front() == '"') 
    {
      auto pos = line.find('"', 1u);
      if (pos == std::string_view::npos) 
      {
        report_error("Can't find matching \" character.");
        return *this;
      }
      tokens.emplace_back(line.substr(1u, pos - 1u));
      line.remove_prefix(pos + 1u);
    }
  }

}