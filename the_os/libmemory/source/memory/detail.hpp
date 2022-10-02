#pragma once

namespace memory::detail
{
  template <typename A>
  concept allocator_like = requires (A&& a, std::size_t s, void* p) 
  {
    { a.allocate(s) } -> std::convertible_to<void*>;
    { a.deallocate(p) };    
  };
}
