#pragma once

#include <list>
#include <tuple>

#include <memory/buffer.hpp>

namespace netboot
{
  struct resource_list
  {    
    auto emplace (std::string_view designator_v, memory::buffer<std::byte> buffer_v) -> bool;
    auto find (std::string_view designator_v) -> memory::buffer<std::byte> const *;
    auto erase (std::string_view designator_v) -> bool;

  private:
    using node_type = std::tuple<std::string, memory::buffer<std::byte>>;

    std::list<node_type> m_list;
  };
}
