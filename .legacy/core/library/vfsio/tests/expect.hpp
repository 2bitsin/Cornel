#pragma once
#include <iostream>
#include <string>
#include <textio/format.hpp>

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
        result += textio::fmt::format_as<"\\x{:02x}", std::string>(unsigned(chr));
        break;
    }   
  }
  return result;
}

#define expect_eq(x, y) \
  return !([&] { \
    if (x != y) { \
      std::cerr << "Expected " #x " == \"" << make_printable_string(y) << "\",\n" \
                   " Instead " #x " == \"" << make_printable_string(x) << "\"\n"; \
      return false; \
    } \
    return true; \
  })()
  