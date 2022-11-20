#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <concepts>
#include <memory>
#include <memory_resource>
#include <algorithm>
#include <utility>
#include <new>

#include <memory/detail.hpp>
#include <textio/logger.hpp>

namespace memory
{
  declare_module(Memory);
  
  [[noreturn]]
  static inline auto throw_bad_cast(std::string_view what)
  {
    Gmod.fatal<"Bad cast ({})">(what);
#if defined(__cpp_exceptions)
    throw std::bad_cast{};
#else    
    std::abort();
#endif
  }


  template <typename T, detail::memory_resource_like A>
  static inline auto allocate_buffer_of(A&& a, std::size_t size) -> std::span<T>
  {
    return std::span<T>{ (T*)a.allocate(size*sizeof(T)), size };
  }   

  template <typename T, detail::memory_resource_like A>
  static inline auto allocate_buffer_of(A&& a, std::size_t size, T const& value) -> std::span<T>
  {
    auto buffer = allocate_buffer_of<T>(a, size);
    for (auto&& single_element : buffer)
      std::construct_at(&single_element, value);
    return buffer;
  }

  template <typename T, typename F, detail::memory_resource_like A>
  requires (std::is_invocable_v<F, std::size_t>)
  static inline auto allocate_buffer_of(A&& a, std::size_t size, F&& fill) -> std::span<T>
  {
    auto buffer = allocate_buffer_of<T>(a, size);
    for (std::size_t i = 0u; i < size; ++i)
      std::construct_at(&buffer[i], fill(i));
    return buffer;
  }

  template <typename T, detail::memory_resource_like A>
  static inline auto deallocate_buffer(A&& a, std::span<T> const& buffer) -> void
  {
    std::destroy(buffer.begin(), buffer.end());
    a.deallocate(buffer.data(), buffer.size()*sizeof(T));
  }

  static constexpr auto reallocate_force_copy_flag = 0x1u;
  static constexpr auto reallocate_dont_release_prev_flag = 0x2u;

  template <typename T, detail::memory_resource_like A>
  static inline auto reallocate_buffer(A&& a, std::span<T> buffer, std::size_t new_size, T const& defval = T(), std::uint32_t flags = 0u) -> std::span<T>
  {    
    if (new_size <= buffer.size() && !(flags & reallocate_force_copy_flag))
      return buffer.subspan(0, new_size);
    auto new_buffer = allocate_buffer_of<T>(a, new_size, defval);
    const auto min_size = std::min(buffer.size(), new_size);
    std::copy_n(buffer.begin(), min_size, new_buffer.begin());
    if(!(flags & reallocate_dont_release_prev_flag))
      deallocate_buffer(a, buffer);
    return new_buffer;
  }
  
  template <typename T, ::memory::detail::memory_resource_like Allocator = std::pmr::memory_resource>
  struct buffer
  {
    using allocator_type = Allocator;
    using value_type = T;

    inline buffer()  
    : m_allocator { *std::pmr::get_default_resource() }
    , m_buffer_sp { }
    {}

    inline buffer(std::size_t size)
    : buffer (*std::pmr::get_default_resource(), size)
    {}

    inline buffer(allocator_type& allocator, std::size_t size)
    : m_allocator { &allocator }
    , m_buffer_sp { allocate_buffer_of<T>(*m_allocator, size) }
    {}

    inline buffer(const buffer& prev)
    : buffer(*prev.m_allocator, prev.size())
    {
      std::copy(prev.begin(), prev.end(), begin());
    }

    inline buffer(buffer&& prev) 
    : m_allocator { std::exchange (prev.m_allocator, nullptr) }
    , m_buffer_sp { std::exchange (prev.m_buffer_sp, std::span<T>{ }) }
    { }

    inline auto resize(std::size_t new_size, T const& defval = T()) -> void
    {
      m_buffer_sp = reallocate_buffer(*m_allocator, m_buffer_sp, new_size, defval);
    }

    inline auto operator = (buffer&& prev) -> buffer& 
    {
      if (this != &prev)
      {
        buffer tmp { std::move (prev) };
        swap(tmp);
      }
      return *this;
    }

    inline auto operator = (const buffer& prev) -> buffer& 
    { 
      if (this != &prev)
      {
        std::destroy_at(this);
        std::construct_at(this, prev);
      }
      return *this;
    }

    inline void swap(buffer& other) 
    {
      std::swap(m_allocator, other.m_allocator);
      std::swap(m_buffer_sp, other.m_buffer_sp);
    }

    inline ~buffer()
    {
      if (nullptr != m_allocator && nullptr != m_buffer_sp.data() && !m_buffer_sp.empty())
      {
        ::memory::deallocate_buffer(*m_allocator, m_buffer_sp);
        m_buffer_sp = std::span<T>{ };
        m_allocator = nullptr;
      }
    }

    inline auto allocate(allocator_type& allocator, std::size_t size) -> void
    {
      std::destroy_at(this);
      std::construct_at(this, allocator, size);
    }

    inline auto size () const  -> std::size_t { return m_buffer_sp.size(); }  
    inline auto empty() const  -> bool { return m_buffer_sp.empty(); }
    
    inline auto data () const  { return m_buffer_sp.data(); }
    inline auto begin() const  { return m_buffer_sp.begin(); }
    inline auto end  () const  { return m_buffer_sp.end(); }

    inline auto data ()  { return m_buffer_sp.data(); }
    inline auto begin()  { return m_buffer_sp.begin(); }
    inline auto end  ()  { return m_buffer_sp.end(); }

    template <typename Q>
    requires (std::is_trivial_v<Q>)
    inline auto as () -> Q&
    {
      if (sizeof(Q) > size() * sizeof (value_type))
        throw_bad_cast(__func__);
      return *std::launder((Q *)data());
    }

    template <typename Q>
    requires (std::is_trivial_v<Q>)
    inline auto as () const -> Q const&
    {
      if (sizeof(Q) > size() * sizeof (value_type))
        throw_bad_cast(__func__);
      return *std::launder((Q const*)data());
    }

    template <typename Q>
    requires (std::is_trivial_v<Q>)
    inline auto as_array (std::size_t count_v) const -> std::span<const Q>
    {
      if (sizeof(Q) * count_v > size() * sizeof (value_type))
        throw_bad_cast(__func__);
      return std::span<const Q>(std::launder((Q const*)data()), count_v);
    }

    template <typename Q>
    requires (std::is_trivial_v<Q>)
    inline auto as_array (std::size_t count_v) -> std::span<Q>
    {
      if (sizeof(Q) * count_v > size() * sizeof (value_type))
        throw_bad_cast(__func__);
      return std::span<const Q>(std::launder((Q *)data()), count_v);
    }
    
    template <typename Q>
    requires (std::is_trivial_v<Q>)
    inline auto as_array () const -> std::span<const Q>
    {
      return as_array<Q>((sizeof(value_type) * size()) / sizeof(Q));
    }

    template <typename Q>
    requires (std::is_trivial_v<Q>)
    inline auto as_array () -> std::span<Q>
    {
      return as_array<Q>((sizeof(value_type) * size()) / sizeof(Q));
    }

    template <typename... Args>
    inline auto subspan(Args&&... args) const  -> std::span<const T>
    { return m_buffer_sp.subspan(std::forward<Args>(args)...); }

    template <typename... Args>
    inline auto subspan(Args&&... args)  -> std::span<T>
    { return m_buffer_sp.subspan(std::forward<Args>(args)...); }

    inline operator std::span<const value_type> () const 
    { return m_buffer_sp; }

    inline operator std::span<value_type> () 
    { return m_buffer_sp; }

    template <typename Q>
    requires (sizeof(Q) == sizeof(value_type))  
    inline operator std::basic_string_view<Q> () const 
    { return std::basic_string_view<Q> { (Q const*)m_buffer_sp.data(), m_buffer_sp.size() }; }

    inline auto operator [] (std::size_t index) const  -> const value_type&
    { return m_buffer_sp[index]; }

    inline auto operator [] (std::size_t index)  -> value_type&
    { return m_buffer_sp[index]; }

  private:
    allocator_type*         m_allocator;
    std::span<value_type>   m_buffer_sp;
  }; 

}