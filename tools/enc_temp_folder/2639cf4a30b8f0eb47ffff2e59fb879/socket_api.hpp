#pragma once

#include <cstdint>
#include <cstddef>

#include <string_view>
#include <tuple>

void v4_init_sockaddr(struct sockaddr_in& target, std::size_t len, const struct v4_address& source);
void v4_init_sockaddr(struct sockaddr& target, std::size_t len, const struct v4_address& source);
auto v4_parse_address(std::string_view what) -> uint32_t;
auto v4_parse_address_and_port(std::string_view what) -> std::pair<uint32_t, uint16_t>;
auto v4_address_to_string(std::uint32_t) -> std::string;