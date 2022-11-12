#include <vfsio/womfs.hpp>

#include <mem>

using vfsio::INode;
using vfsio::IStream;
using vfsio::IFile;
using vfsio::IDirectoryEntry;
using vfsio::IDirectory;
using vfsio::IVolume;
using vfsio::error;
using vfsio::node_type;
using vfsio::relative_to;
using vfsio::allocator_type;
using vfsio::WomfsCreate;

////////////////
// WomfsVolume
////////////////

struct WomfsVolume: 
	public IVolume
{
	WomfsVolume (allocator_type* allocator_v, error& error_v, std::size_t size_init_v, std::size_t size_limit_v)
	:	m_allocator  { allocator_v  }
	, m_size_limit { size_limit_v }
	, m_storage    { allocator_v  }
	{
	
	}
	
 ~WomfsVolume()
  {
	
  }

	auto type(error& error_v) const -> node_type override
	{
		return node_type::volume;
	}

private:
	allocator_type* m_allocator;
	std::size_t m_size_limit;
	std::span<std::byte> m_storage;
};




auto vfsio::WomfsCreate (error& error_v, std::size_t size_init_v, std::size_t size_limit_v) -> IVolume*
{ 
	if (nullptr == memory_resource_v)
		memory_resource_v = std::pmr::get_default_resource(); 
	auto volume_p = std::make_unique<WomfsVolume>(memory_resource_v, error_v, size_limit_v);
	if (error_v != error::none)
		return nullptr;
	return volume_p.release();
}

