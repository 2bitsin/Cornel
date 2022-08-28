#include <print.hpp>

#include <cstddef>
#include <span>
#include <ranges>
#include <algorithm>

static void (*output_fun)(char) = nullptr;

void print_attach_output(void (*_output_fun)(char))
{ output_fun = _output_fun; }

void print_detach_output(void (*_output_fun)(char))
{}

void print(char value)
{
  if (output_fun != nullptr)
  {
    output_fun(value);
  }
}

void print(std::string_view value)
{
  for (auto chr : value) {
    print(chr);
  }
}

void print(std::initializer_list<std::string_view> values)
{
  for (auto value : values) {
    print(value);
  }
}