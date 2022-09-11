#pragma once
#include <iostream>
#include <string>
#include <format>

static inline auto make_printable_string(auto&& value) -> std::string
{
  std::string result;
  for (auto chr : value)
  {
    if (chr >= ' ' && chr < 0x7f) 
    {
      result += chr;
      continue;
    }

    switch (chr)
    {
    case '\a': result += "\\a"; break;
    case '\b': result += "\\b"; break;
    case '\f': result += "\\f"; break;
    case '\n': result += "\\n"; break;
    case '\r': result += "\\r"; break;
    case '\t': result += "\\t"; break;
    case '\v': result += "\\v"; break;
    case '\\': result += "\\\\"; break;
    case '\'': result += "\\'"; break;
    case '\"': result += "\\\""; break;
    default: 
        result += std::format("\\x{:02x}", unsigned(chr));
        break;
    }   
  }
  return result;
}

  