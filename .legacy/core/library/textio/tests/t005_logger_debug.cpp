#include <textio/format.hpp>
#include <textio/logger.hpp>

#include <textio/format/helpers/data_size.hpp>
#include <textio/format/helpers/repeat_value.hpp>

declare_module(Interrupts);

static inline constexpr std::string_view G_exception_string [] = 
{
  "Divide-by-zero",
  "Debug",
  "Non-maskable Interrupt",
  "Breakpoint",
  "Overflow",
  "Bound Range Exceeded",
  "Invalid Opcode",
  "Device Not Available",
  "Double Fault",
  "Coprocessor Segment Overrun",
  "Invalid TSS",
  "Segment Not Present",
  "Stack-Segment Fault",
  "General Protection Fault",
  "Page Fault",
  "",
  "x87 Floating-Point Exception",
  "Alignment Check",
  "Machine Check",
  "SIMD Floating-Point Exception",
  "Virtualization Exception",
  "Control Protection Exception",
  "",
  "",
  "",
  "",
  "",
  "",
  "Hypervisor Injection Exception",
  "VMM Communication Exception",
  "Security Exception",
  ""
};


int main(int,char**) {
  using textio::fmt::helpers::data_size;
	using textio::fmt::helpers::repeat_value;

  Glog.info(repeat_value<79>('*'));
  Glog.info<"EXCEPTION #{:02x} ({:s})">(0x0D, G_exception_string[0x0D]);
  Glog.info(repeat_value<79>('*'));

  Glog.info<" EAX={:08x} EBX={:08x} ECX={:08x} EDX={:08x}">(0x60000001, 0x12345689, 0x12345689, 0x12345689);
	volatile  auto i = sizeof(textio::fmt::detail::format_options<char>);
	
  return 0;
}