#pragma once

namespace memory::detail
{
  template <typename A>
  concept allocator_like = requires (A&& a, void* p, std::size_t s) 
  {
    { a.allocate(s) } -> std::convertible_to<void*>;
    { a.deallocate(p, s) };    
  };
}
