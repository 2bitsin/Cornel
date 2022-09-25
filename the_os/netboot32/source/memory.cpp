#include <utils/macros.hpp>
#include <utils/debug.hpp>

#include <memory/block_list.hpp>
#include <memory/allocate_buffer.hpp>

#include <hardware/console.hpp>
#include <hardware/atwenty.hpp>
#include <hardware/x86gdt.hpp>
#include <hardware/x86flags.hpp>
#include <hardware/x86real_addr.hpp>
#include <hardware/x86bios.hpp>

#include <netboot32/memory.hpp>
#include <netboot32/panick.hpp>

#include <cstdlib>
#include <algorithm>

extern "C"
{
  std::byte* volatile G_heap_begin;  
}

static block_list G_base_heap;
static block_list G_extended_heap;

static void initialize_extended_heap() 
{
  // Initialize extended memory heap
  x86arch::bios_acpi_memory_map_entry_t entry;
  std::uint32_t i_offset = 0u, o_offset = 0u, length = 0u;
  std::uint64_t heap_size = 0u;

  // Must enable A20 line to access extended memory
  if (!atwenty::try_enable()) {
    panick::cant_enable_atwenty();
  }

  do
  {
    i_offset = o_offset;
    if (std::errc{} != x86arch::bios_acpi_memory_map_read(entry, length, o_offset, i_offset)) 
      break;
    if (length < 20 || length > 24)
      break;

    // Is available memory?
    if (entry.type == 1u)
    {                   
      G_extended_heap.insert_range({(std::byte*)entry.base, (std::size_t)entry.size }); 
      heap_size += entry.size;
    }
  } 
  while(o_offset != 0u);
  console::writeln("  * ", heap_size, " bytes extended memory available.");  
}

static void initialize_base_heap()
{
  const auto* top_of_heap = (std::byte const *)(bda::conventional_memory_size * 0x400u);
  std::span heap_bytes { G_heap_begin, top_of_heap };
  // Initialize heap
  G_base_heap.insert_range(heap_bytes);
  console::writeln("  * ", heap_bytes.size(), " bytes base memory available."); 
}

void memory::initialize(bool first_time)
{
  using namespace textio::simple::fmt;
  if (!first_time)
    return;
  console::writeln("Initializing heap ...");  
  initialize_base_heap();
  initialize_extended_heap();
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
  auto ptr = G_base_heap.allocate(size);
  if (nullptr == ptr)
  {
    panick::out_of_memory(size, G_base_heap);
  }
  return ptr;
}

CO_PUBLIC 
void* calloc(std::size_t nelem, std::size_t size)
{
  // Get total size
  size *= nelem;
  // Allocate memory
  auto ptr = G_base_heap.allocate(size);
  // If successful, fill with zeros
  if (nullptr != ptr) 
  {
    // Zero out new block
    __builtin_memset(ptr, 0, size);
  }
  else 
  {
    panick::out_of_memory(size, G_base_heap);
  }
  return ptr;
}

CO_PUBLIC
void free(void* ptr)
{
  if(!G_base_heap.deallocate(ptr))
    panick::invalid_free(ptr, G_base_heap);
}

CO_PUBLIC 
void* realloc(void* old_ptr, std::size_t new_size)
{  
  if (nullptr == old_ptr) 
  {
    // realloc(NULL, size) is equivalent to malloc(size)
    return G_base_heap.allocate(new_size);  
  }

  // Try to resize block
  if (auto new_ptr = G_base_heap.reallocate(old_ptr, new_size); nullptr != new_ptr)
  {
    // Success
    return new_ptr;
  }

  // Allocate new block and copy data
  if (auto new_ptr = G_base_heap.allocate(new_size); nullptr != new_ptr) 
  {
    // Get old block size
    auto old_size = G_base_heap.size(old_ptr);
    // Copy data
    __builtin_memcpy(new_ptr, old_ptr,std::min(new_size, old_size));
    // Free old block
    G_base_heap.deallocate(old_ptr);
    return new_ptr;
  }
  
  panick::out_of_memory(new_size, G_base_heap);
}

void operator delete(void* ptr) noexcept
{
  if(!G_base_heap.deallocate(ptr))
    panick::invalid_free(ptr, G_base_heap);
}

void operator delete[](void* ptr) noexcept
{
  if(!G_base_heap.deallocate(ptr))
    panick::invalid_free(ptr, G_base_heap);
}

void operator delete(void* ptr, [[maybe_unused]] std::size_t size) noexcept
{
  if(!G_base_heap.deallocate(ptr))
    panick::invalid_free(ptr, G_base_heap);
}

void operator delete[](void* ptr, [[maybe_unused]] std::size_t size) noexcept
{
  if(!G_base_heap.deallocate(ptr))
    panick::invalid_free(ptr, G_base_heap);
}

void* operator new[](std::size_t size) noexcept
{
  auto ptr = G_base_heap.allocate(size);
  if (nullptr == ptr) 
    panick::out_of_memory(size, G_base_heap);
  return ptr;
}

void* operator new(std::size_t size) noexcept
{
  auto ptr = G_base_heap.allocate(size);
  if (nullptr == ptr) 
    panick::out_of_memory(size, G_base_heap);
  return ptr;
}
