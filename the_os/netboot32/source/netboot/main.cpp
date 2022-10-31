#include <netboot/netboot.hpp>
#include <netboot/interrupts.hpp>
#include <netboot/runtime.hpp>
#include <netboot/memory.hpp>

static netboot::Netboot* G_netboot { nullptr };

extern "C"
void main ()
{      
  using netboot::Netboot;

  runtime::initialize(true);
  interrupts::initialize(true);    
  memory::initialize(true);

  G_netboot = new Netboot();
  G_netboot->initialize();

  G_netboot->main();

  G_netboot->finalize();  
  delete G_netboot;

  std::abort();

  memory::finalize(true);  
  interrupts::finalize(true);
  runtime::finalize(true);  

}
