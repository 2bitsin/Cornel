#include <textio/format.hpp>
#include <textio/logger.hpp>

#include <textio/format/helpers/data_size.hpp>

declare_module(Memory);

int main(int,char**) {
	using textio::fmt::helpers::data_size;

  Gmod.info<"{} bytes base memory available.">(data_size(12212411)); 

	return 0;
}