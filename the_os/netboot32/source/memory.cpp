#include <misc/macros.hpp>

#include <memory/block_list.hpp>
#include <memory/allocate_buffer.hpp>

#include <hardware/console.hpp>
#include <hardware/atwenty.hpp>
#include <hardware/x86arch.hpp>

#include <netboot32/memory.hpp>
#include <netboot32/panick.hpp>

#include <cstdlib>
#include <algorithm>

extern "C"
{
  std::byte* volatile G_heap_begin;  
}

static block_list G_heap;

void memory::initialize(bool first_time)
{
  if (!first_time)
    return;
  
  const auto* top_of_heap = (std::byte const *)(bda::conventional_memory_size * 0x400u);
  std::span heap_bytes { G_heap_begin, top_of_heap };

  // Initialize heap
  G_heap.initialize(heap_bytes);
  console::writeln("Initializing heap, ",  heap_bytes.size(), " bytes available.");  

  if (!atwenty::try_enable()) 
    panick::cant_enable_atwenty();

  __debugbreak();
  x86arch::gdt_resize(x86arch::gdt_size(), x86arch::gdt_make32({}), reallocate_force_copy_flag|reallocate_dont_release_prev_flag);  
  __debugbreak();
}

void memory::finalize(bool last_time)
{
  if (!last_time)
    return;
}

using namespace std;

CO_PUBLIC 
void* malloc(std::size_t size)
{
  auto ptr = G_heap.allocate(size);
  if (nullptr == ptr)
  {
    panick::out_of_memory(size, G_heap);
  }
  return ptr;
}

CO_PUBLIC 
void* calloc(std::size_t nelem, std::size_t size)
{
  // Get total size
  size *= nelem;
  // Allocate memory
  auto ptr = G_heap.allocate(size);
  // If successful, fill with zeros
  if (nullptr != ptr) 
  {
    // Zero out new block
    __builtin_memset(ptr, 0, size);
  }
  else 
  {
    panick::out_of_memory(size, G_heap);
  }
  return ptr;
}

CO_PUBLIC
void free(void* ptr)
{
  if(!G_heap.deallocate(ptr))
    panick::invalid_free(ptr, G_heap);
}

CO_PUBLIC 
void* realloc(void* old_ptr, std::size_t new_size)
{  
  if (nullptr == old_ptr) 
  {
    // realloc(NULL, size) is equivalent to malloc(size)
    return G_heap.allocate(new_size);  
  }

  // Try to resize block
  if (auto new_ptr = G_heap.reallocate(old_ptr, new_size); nullptr != new_ptr)
  {
    // Success
    return new_ptr;
  }

  // Allocate new block and copy data
  if (auto new_ptr = G_heap.allocate(new_size); nullptr != new_ptr) 
  {
    // Get old block size
    auto old_size = G_heap.size(old_ptr);
    // Copy data
    __builtin_memcpy(new_ptr, old_ptr,std::min(new_size, old_size));
    // Free old block
    G_heap.deallocate(old_ptr);
    return new_ptr;
  }
  
  panick::out_of_memory(new_size, G_heap);
}

void operator delete(void* ptr) noexcept
{
  if(!G_heap.deallocate(ptr))
    panick::invalid_free(ptr, G_heap);
}

void operator delete[](void* ptr) noexcept
{
  if(!G_heap.deallocate(ptr))
    panick::invalid_free(ptr, G_heap);
}

void operator delete(void* ptr, [[maybe_unused]] std::size_t size) noexcept
{
  if(!G_heap.deallocate(ptr))
    panick::invalid_free(ptr, G_heap);
}

void operator delete[](void* ptr, [[maybe_unused]] std::size_t size) noexcept
{
  if(!G_heap.deallocate(ptr))
    panick::invalid_free(ptr, G_heap);
}

void* operator new[](std::size_t size) noexcept
{
  auto ptr = G_heap.allocate(size);
  if (nullptr == ptr) 
    panick::out_of_memory(size, G_heap);
  return ptr;
}

void* operator new(std::size_t size) noexcept
{
  auto ptr = G_heap.allocate(size);
  if (nullptr == ptr) 
    panick::out_of_memory(size, G_heap);
  return ptr;
}

auto memory::is_valid(void* ptr) -> bool
{
  return G_heap.is_valid(ptr);
}