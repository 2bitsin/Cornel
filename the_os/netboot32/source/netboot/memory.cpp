#include <utils/macros.hpp>
#include <utils/debug.hpp>

#include <memory/block_list.hpp>
#include <memory/buffer.hpp>

#include <hardware/ibm/atwenty.hpp>
#include <hardware/x86/address16.hpp>
#include <hardware/x86/bios.hpp>

#include <netboot/memory.hpp>
#include <netboot/panick.hpp>

#include <textio/format/helpers/data_size.hpp>
#include <textio/logger.hpp>

#include <cstdlib>
#include <algorithm>

declare_module(Memory);

using textio::fmt::helpers::data_size;

extern "C"
{
  std::byte* volatile G_heap_begin;  
}

static block_list G_base_heap;
static block_list G_extended_heap;

static void initialize_extended_heap() 
{
  using namespace textio::fmt;

  // Initialize extended memory heap
  x86arch::bios_acpi_memory_map_entry_t entry;
  std::uint32_t i_offset = 0u, o_offset = 0u, length = 0u;
  std::uint64_t heap_size = 0u;

  // Must enable A20 line to access extended memory
  if (!atwenty::try_enable()) 
  { 
    Gmod.fatal<"Failed to enable address line 20.">();
    std::abort();
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
  using namespace textio::fmt::helpers;
  
  Gmod.info<"{} bytes extended memory available.">(data_size(heap_size));
}

static void initialize_base_heap()
{
  using x86arch::bda;
  using namespace textio::fmt::helpers;

  const auto* top_of_heap = (std::byte const *)(bda().base_memory_size * 0x400u);
  std::span heap_bytes { G_heap_begin, top_of_heap };
  // Initialize heap
  G_base_heap.insert_range(heap_bytes);

  Gmod.info<"{} bytes base memory available.">(data_size(heap_bytes.size())); 
}


using namespace std;

CO_PUBLIC CO_NOINLINE 
void* malloc(std::size_t size)
{
  auto ptr = G_base_heap.allocate(size);
  if (nullptr == ptr)
  { 
    Gmod.trace(__func__);
    Gmod.fatal<"Out of base memory, unable to allocate {} bytes">(data_size(size));
    Gmod.debug(G_base_heap);
    std::abort();
  }
  return ptr;
}

CO_PUBLIC CO_NOINLINE 
void* calloc(std::size_t nelem, std::size_t size)
{
  // Get total size
  size *= nelem;
  // Allocate memory
  auto ptr = G_base_heap.allocate(size);
  // If successful, fill with zeros
  if (nullptr != ptr) 
  { __builtin_memset(ptr, 0, size); }
  else 
  { 
    Gmod.trace(__func__);
    Gmod.fatal<"Out of base memory, unable to allocate {} bytes">(data_size(size));
    Gmod.debug(G_base_heap);
    std::abort();
  }
  return ptr;
}

CO_PUBLIC CO_NOINLINE
void free(void* ptr)
{
  if(!G_base_heap.deallocate(ptr))
  { 
    Gmod.trace(__func__);
    Gmod.fatal<"Unable to free memory block at {:#08p}">(ptr);
    Gmod.debug(G_base_heap);
    std::abort();
  }
}

CO_PUBLIC CO_NOINLINE 
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

  Gmod.trace(__func__);  
  Gmod.fatal<"Out of base memory, unable to allocate {} bytes">(data_size(new_size));
  Gmod.debug(G_base_heap);
  std::abort();  
}

CO_NOINLINE
void operator delete(void* ptr) 
{
  if(!G_base_heap.deallocate(ptr))
  { 
    Gmod.trace(__func__);
    Gmod.fatal<"Unable to free memory block at {:#08p}">(ptr);
    Gmod.debug(G_base_heap);
    std::abort();
  }
}

CO_NOINLINE
void operator delete[](void* ptr) 
{
  if(!G_base_heap.deallocate(ptr))
  { 
    Gmod.trace(__func__);
    Gmod.fatal<"Unable to free memory block at {:#08p}">(ptr);
    Gmod.debug(G_base_heap);
    std::abort();
  }
}

CO_NOINLINE
void operator delete(void* ptr, [[maybe_unused]] std::size_t size) 
{
  if(!G_base_heap.deallocate(ptr))
  { 
    Gmod.trace(__func__);
    Gmod.fatal<"Unable to free memory block at {:#08p}">(ptr);
    Gmod.debug(G_base_heap);
    std::abort();
  }
}

CO_NOINLINE
void operator delete[](void* ptr, [[maybe_unused]] std::size_t size) 
{
  if(!G_base_heap.deallocate(ptr))
  { 
    Gmod.trace(__func__);
    Gmod.fatal<"Unable to free memory block at {:#08p}">(ptr);
    Gmod.debug(G_base_heap);
    std::abort();
  }
}

CO_NOINLINE
void* operator new[](std::size_t size) 
{
  auto ptr = G_base_heap.allocate(size);
  if (nullptr == ptr) 
  { 
    Gmod.trace(__func__);
    Gmod.fatal<"Out of base memory, unable to allocate {} bytes">(data_size(size));
    Gmod.debug(G_base_heap);
    std::abort();
  }
  return ptr;
}

CO_NOINLINE
void* operator new(std::size_t size) 
{
  auto ptr = G_base_heap.allocate(size);
  if (nullptr == ptr) 
  { 
    Gmod.trace(__func__);
    Gmod.fatal<"Out of base memory, unable to allocate {} bytes">(data_size(size));
    Gmod.debug(G_base_heap);
    std::abort();
  }
  return ptr;
}

namespace memory
{
  void initialize(bool first_time)
  {
    using namespace textio::fmt;
    if (!first_time)
      return;
  
    Gmod.debug<"Initializing heap ...">();   
  
    initialize_base_heap();
    initialize_extended_heap();
    std::pmr::set_default_resource(&::memory::get_base_heap());
  }

  void finalize(bool last_time)
  {
    if (!last_time)
      return;
  }

  auto ext_allocate(std::size_t size) -> void*
  {
    auto pointer = G_extended_heap.allocate(size);
    if (nullptr == pointer)
    { 
      Gmod.trace(__func__); 
      Gmod.fatal<"Out of extended memory, unable to allocate {} bytes">(data_size(size));
      Gmod.debug(G_extended_heap);
      std::abort();
    }
    return pointer;
  }

  auto ext_deallocate(void* pointer) -> void
  {
    if (!G_extended_heap.deallocate(pointer))
    { 
      Gmod.trace(__func__);
      Gmod.fatal<"Unable to free memory block at {:#08p}">(pointer);
      Gmod.debug(G_extended_heap);
      std::abort();
    }
  }

  
  auto get_base_heap()  -> std::pmr::memory_resource&
  {    
    class resource_impl
    : public std::pmr::memory_resource
    {
    private:    
      CO_NOINLINE
      void* do_allocate(std::size_t size, std::size_t alignment) override {
        if (alignment > 16) 
        {
          Gmod.trace(__func__);
          Gmod.fatal<"unable to satisfy alignment of more then 16bytes">();
          std::abort();
        }          
        return std::malloc(size);
      }

      CO_NOINLINE
      void do_deallocate(void* ptr, std::size_t, std::size_t alignment) override {
        if (alignment > 16) 
        {
          Gmod.trace(__func__);
          Gmod.fatal<"unable to satisfy alignment of more then 16bytes">();
          std::abort();
        }
        std::free(ptr);
      }

      bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override 
      { return this == &other; }
    };
    static resource_impl resource;
    return resource;
  }

  auto get_extended_heap()  -> std::pmr::memory_resource&
  {
    class resource_impl
    : public std::pmr::memory_resource
    {
    private:
      CO_NOINLINE
      void* do_allocate(std::size_t size, std::size_t alignment) override {
        if (alignment > 16)
        {
          Gmod.trace(__func__);
          Gmod.fatal<"unable to satisfy alignment of more then 16bytes">();
          std::abort();
        }
        return ext_allocate(size);
      }

      CO_NOINLINE
      void do_deallocate(void* ptr, std::size_t, std::size_t alignment) override {
        if (alignment > 16) 
        {
          Gmod.trace(__func__);
          Gmod.fatal<"unable to satisfy alignment of more then 16bytes">();
          std::abort();
        }
        ext_deallocate(ptr);
      }

      bool do_is_equal(const std::pmr::memory_resource& other) const noexcept override 
      { return this == &other; }
    };
    static resource_impl resource;
    return resource;
  }
}

static std::pmr::memory_resource* G_default_resource = nullptr;

namespace std::pmr
{
  memory_resource::~memory_resource() {}

  auto set_default_resource(memory_resource* resource)  -> memory_resource*
  {
    return std::exchange(G_default_resource, resource);
  }

  auto get_default_resource()  -> memory_resource*
  {
    return G_default_resource;
  }

  auto new_delete_resource()  -> memory_resource*
  {
    return &memory::get_base_heap();
  }

  auto monotonic_buffer_resource::_M_new_buffer(std::size_t, std::size_t) -> void
  {
    Gmod.trace(__func__);
    Gmod.fatal<"monotonic_buffer_resource::_M_new_buffer not implemented">();
    std::abort();
  }
}
