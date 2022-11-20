#pragma once

#include <vfsio/vfsio.hpp>

#include <cstddef>
#include <cstdint>
#include <limits>
#include <memory>
#include <type_traits>

namespace vfsio
{
  namespace detail
  {
    struct DummyHeap
    {
      auto allocate(std::size_t) noexcept -> void* { return nullptr; };
      auto deallocate(void*, std::size_t) noexcept -> bool { return false; };
      auto reallocate(void*, std::size_t) noexcept -> void* { return nullptr; };
    };

    struct IHeap
    {
      // Not including aligment parameter, we're just gonna assume 16byte aligment and that is it
      virtual auto allocate   (error& error_v, std::size_t size_v) noexcept -> void* = 0;
      virtual auto deallocate (error& error_v, void*, std::size_t size_v) noexcept -> bool = 0;
      virtual auto reallocate (error& error_v, void*, std::size_t size_v) noexcept -> void* = 0;
    };

    template <typename T>
    struct Heap_wrapper final: 
      public IHeap
    {
      Heap_wrapper(T& heap_v)
      : m_heap(heap_v)      
      {}

      auto allocate (error& error_v, std::size_t size_v) noexcept -> void* override 
      {
        error_v = error::none;
        const auto pointer_v = m_heap.allocate (size_v);
        if (nullptr == pointer_v) {
          error_v = error::out_of_memory;         
        }
        return pointer_v;
      }
      
      auto deallocate (error& error_v, void* pointer_v, std::size_t size_v) noexcept -> bool override
      {
        error_v = error::none;
        if (!m_heap.deallocate(pointer_v, size_v)) {
          error_v = error::invalid_argument;
          return false;
        }
        return true;
      }

      auto reallocate (error& error_v, void* pointer_v, std::size_t size_v) noexcept -> void* override
      {
        error_v = error::none;
        pointer_v = m_heap.reallocate(pointer_v, size_v);
        if (nullptr == pointer_v) {
          error_v = error::out_of_memory;         
        }
        return pointer_v;
      }
      
    private:
      T& m_heap;
    };
    
    
  }

  struct heapfile final: 
    public vfsio::helper::IFile
  {
    template <typename HeapT>   
    heapfile(error& error_v, HeapT& heap_v,        
      std::size_t max_size_v = std::numeric_limits<std::size_t>::max())   
    : m_wstore   { }
    , m_heap_w   { *new (&m_wstore) detail::Heap_wrapper<HeapT>(heap_v) }
    , m_data_s   { }
    , m_max_size { max_size_v }
    {
      error_v = error::none;
    }
    
    using vfsio::helper::IFile::read;
    using vfsio::helper::IFile::write;

    auto read(error& error_v, std::span<std::byte> buffer_v, std::uintmax_t offset_v) -> std::span<std::byte> override;
    auto write(error& error_v, std::span<std::byte const> buffer_v, std::uintmax_t offset_v) -> std::span<std::byte const> override;
    auto resize(error& error_v, std::uintmax_t size_v) -> std::uintmax_t override;
    auto size(error& error_v) const -> std::uintmax_t override;

  private:
    std::aligned_union_t<0, detail::Heap_wrapper<detail::DummyHeap>> m_wstore;
    detail::IHeap& m_heap_w;
    std::span<std::byte> m_data_s;
    std::size_t m_max_size;
  };
}