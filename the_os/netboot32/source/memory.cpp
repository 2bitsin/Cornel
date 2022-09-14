#include <misc/macros.hpp>
#include <hardware/console.hpp>
#include <memory/block_list.hpp>
#include <netboot32/memory.hpp>


#include <cstdlib>
#include <algorithm>

extern "C"
{
  std::byte* volatile G_heap_begin;  
}

static block_list G_heap;

[[noreturn]] 
static void panick_out_of_memory()
{
  console::writeln("#002 - Out of memory!");
  std::abort();
}

[[noreturn]] 
static void panick_invalid_free()
{
  console::writeln("#003 - Heap deallocation failed, possible corruption.");
  std::abort();
}

namespace std
{
  void __throw_bad_alloc()
  {
    console::writeln("#005 - Bad allocation error");
    std::abort();
  }


  void __throw_length_error(char const* err)
  {
    console::writeln("#004 - Length error, ", err);
    std::abort();
  }
}


void memory::initialize(bool first_time)
{
  if (!first_time)
    return;
  console::writeln("Available heap size : ", bda::conventional_memory_size * 1024u - (std::uintptr_t)G_heap_begin, " bytes");  
  // Initialize heap
  G_heap.initialize({ G_heap_begin, G_heap_begin + bda::conventional_memory_size * 1024u });
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
  return G_heap.allocate(size);
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
  return ptr;
}

CO_PUBLIC
void free(void* ptr)
{
  G_heap.deallocate(ptr);
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
  
  panick_out_of_memory();
}

void operator delete(void* ptr) noexcept
{
  if(!G_heap.deallocate(ptr))
    panick_invalid_free();
}

void operator delete[](void* ptr) noexcept
{
  if(!G_heap.deallocate(ptr))
    panick_invalid_free();
}

void operator delete(void* ptr, [[maybe_unused]] std::size_t size) noexcept
{
  if(!G_heap.deallocate(ptr))
    panick_invalid_free();
}

void operator delete[](void* ptr, [[maybe_unused]] std::size_t size) noexcept
{
  if(!G_heap.deallocate(ptr))
    panick_invalid_free();
}

void* operator new[](std::size_t size) noexcept
{
  auto ptr = G_heap.allocate(size);
  if (nullptr == ptr) 
    panick_out_of_memory();
  return ptr;
}

void* operator new(std::size_t size) noexcept
{
  auto ptr = G_heap.allocate(size);
  if (nullptr == ptr) 
    panick_out_of_memory();
  return ptr;
}
