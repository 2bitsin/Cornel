#pragma once

#include <cstdint>
#include <cstddef>

#include <string_view>
#include <tuple>
#include <span>

using int_socket_type = std::intptr_t;

void v4_init_sockaddr(struct sockaddr_in& target, std::size_t len, const struct v4_address& source);
void v4_init_sockaddr(struct sockaddr& target, std::size_t len, const struct v4_address& source);
auto v4_parse_address(std::string_view what) -> uint32_t;
auto v4_parse_address_and_port(std::string_view what) -> std::pair<uint32_t, uint16_t>;
auto v4_parse_address_and_port(struct sockaddr_in const& what) -> std::pair<uint32_t, uint16_t>;
auto v4_address_to_string(std::uint32_t) -> std::string;
auto v4_resolve_single(std::string_view target) -> std::uint32_t;
auto v4_socket_make_udp() -> int_socket_type;
auto v4_socket_make_udp(const struct v4_address& address) -> int_socket_type;
auto v4_socket_make_invalid() -> int_socket_type;
void v4_socket_bind(int_socket_type socket, const struct v4_address& address);
void v4_socket_close(int_socket_type socket);
auto v4_socket_recv(int_socket_type socket, std::span<std::byte>& buffer, struct v4_address& address, std::uint32_t flags) -> std::size_t;
auto v4_socket_send(int_socket_type socket, std::span<const std::byte>& buffer, const struct v4_address& address, std::uint32_t flags) -> std::size_t;
auto mac_address_to_string(std::span<const std::uint8_t> data) -> std::string;