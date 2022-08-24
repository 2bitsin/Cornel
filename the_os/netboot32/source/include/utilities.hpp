#include <cstddef>
#include <cstdint>

template <std::size_t Address, typename T>
auto&& variable_at = *reinterpret_cast<T*>(Address);