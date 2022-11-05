#include <vfsio/idirectory.hpp>    

using vfsio::Idirectory;

auto Idirectory::type() const -> vfsio::device_type 
{
	clear_error();
	return vfsio::device_type::directory;
}


