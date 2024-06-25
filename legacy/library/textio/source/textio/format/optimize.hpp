#pragma once

#include <type_traits>
#include <meta/type_list.hpp>
#include <meta/string.hpp>
#include "node_static.hpp"
#include "node_insert.hpp"

namespace textio::fmt::detail
{
  template <typename Entity> struct format_is_static_impl: std::integral_constant<bool, false> {};
  template <auto Value> struct format_is_static_impl<format_node_static<Value>>: std::integral_constant<bool, true> {};
  template <typename Entity> concept format_is_static = format_is_static_impl<Entity>::value;
  
  template <typename Node0>
  constexpr auto format_optimize_merge(meta::type_list<Node0> const&)
  {
    return meta::type_list<Node0>{};
  }
  
  template <typename Node0, typename Node1, typename... NodeN>
  requires (!format_is_static<Node0> || !format_is_static<Node1>)
  constexpr auto format_optimize_merge(meta::type_list<Node0, Node1, NodeN...> const&)
  {
    return meta::list_prepend<Node0, decltype(format_optimize_merge(meta::type_list<Node1, NodeN...>{}))>{};
  }
  
  template <typename Node0, typename Node1, typename... NodeN>
  requires (format_is_static<Node0> && format_is_static<Node1>)
  constexpr auto format_optimize_merge(meta::type_list<Node0, Node1, NodeN...> const&)
  {
    return format_optimize_merge(meta::type_list<format_node_static<meta::string(Node0::value, Node1::value)>, NodeN...>{});    
  }
  
  template <typename Node0>
  constexpr auto format_optimize_empty(meta::type_list<Node0> const&)
  {
    return meta::type_list<Node0>{};
  }
  
  template <typename Node0, typename... NodeN>
  constexpr auto format_optimize_empty(meta::type_list<Node0, NodeN...> const&)
  {
    return meta::list_prepend<Node0, decltype(format_optimize_empty(meta::type_list<NodeN...>{}))> {};
  }
  
  template <typename Node0, typename... NodeN>
  requires(format_is_static<Node0> && Node0::value.empty())
  constexpr auto format_optimize_empty(meta::type_list<Node0, NodeN...> const&)
  {
    return format_optimize_empty(meta::type_list<NodeN...>{});
  }
}