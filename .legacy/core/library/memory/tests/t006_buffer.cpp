#include <memory/buffer.hpp>

#include <string>
#include <memory>
#include <memory_resource>
#include <cstring>
#include <new>
#include <concepts>
#include <unordered_map>
#include <tuple>

#include "expect.hpp"

template <std::output_iterator<char> I>
class logged_memory_resource: public std::pmr::memory_resource
{
public:
  using output_iterator_type = I;

  inline static std::unordered_map<void*, std::uintptr_t> m_ptrs;
  inline static std::uintptr_t m_index { 0 };
  inline static std::uintmax_t m_used { 0 };

  logged_memory_resource(output_iterator_type& out): m_out(out) {}

  void* do_allocate(std::size_t size, std::size_t align)  override
  {
    using textio::simple::fmt::hex;
    auto ptr = std::pmr::get_default_resource()->allocate(size, align);
    m_ptrs[ptr] = m_index++;
    m_used += size;
    m_out = textio::simple::writeln(m_out, "do_allocate", std::tuple(size, align), " => ", hex<'&'>(m_ptrs[ptr]));
    return ptr;
  }

  void do_deallocate(void* ptr, std::size_t size, std::size_t align)  override 
  {   
    using textio::simple::fmt::hex;
    m_out = textio::simple::writeln(m_out, "do_deallocate", std::tuple(hex<'&'>(m_ptrs[ptr]), size, align));    
    m_used -= size;
    m_ptrs.erase(ptr);
    std::pmr::get_default_resource()->deallocate(ptr, size, align);
  }

  bool do_is_equal(const std::pmr::memory_resource& other) const  override 
  { return this == &other; }

  template <std::output_iterator<char> I>
  static void stats(I& out_i)
  {
    out_i = textio::simple::writeln(out_i, "used .......... : ", m_used);
    out_i = textio::simple::writeln(out_i, "allocations ... : ", m_ptrs.size());    
  }

private:
  output_iterator_type& m_out;
};

template <typename Lmr>
void foo(Lmr&& lmr, ::memory::buffer<std::byte>& output)
{
  ::memory::buffer<std::byte> dut (lmr, 0x40u); 
  std::strncpy ((char*)dut.data(), "Llanfairpwllgwyngyllgogerychwyrndrobwllllantysiliogogogoch!?.@%_", dut.size());   
  output = std::move(dut);
}

int main(int argc, char **grgv)
{ 
  using namespace textio::simple;
  using namespace textio::simple::fmt;
  
  static const std::string expected_output = "do_allocate(32, 8) => 00000000\r\ndo_allocate(32, 8) => 00000001\r\nquinquagintaquadringentillionths\r\nquinquagintaquadringentillionths\r\ndo_allocate(64, 8) => 00000002\r\ndo_deallocate(00000000, 32, 8)\r\nLlanfairpwllgwyngyllgogerychwyrndrobwllllantysiliogogogoch!?.@%_\r\ndo_deallocate(00000001, 32, 8)\r\ndo_deallocate(00000002, 64, 8)\r\nused .......... : 0\r\nallocations ... : 0\r\n";
  std::string output;
  auto out_i = std::back_inserter(output);
    
  using namespace memory;

  logged_memory_resource lmr { out_i };
  {
    buffer<std::byte> dut (lmr, 32);  
    std::strncpy ((char*)dut.data(), "quinquagintaquadringentillionths", dut.size());   
    auto move_dut = std::move(dut);   
    auto copy_dut = move_dut;
    textio::simple::writeln(out_i, std::string_view{ move_dut });
    textio::simple::writeln(out_i, std::string_view{ copy_dut });
    foo(lmr, move_dut);
    textio::simple::writeln(out_i, std::string_view{ move_dut });
  }
  
  lmr.stats(out_i);
  
  expect_eq(output, expected_output);
  
  return 0;
}
