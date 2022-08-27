#pragma once

#include <string_view>
#include <initializer_list>

void display_write(char value);
void display_write(std::string_view value);
void display_write(std::initializer_list<std::string_view> value);
