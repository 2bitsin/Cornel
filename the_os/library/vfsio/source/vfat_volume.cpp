#include <vfsio/vfat_volume.hpp>
#include <memory/buffer.hpp>

#include <cassert>
#include <algorithm>
#include <optional>

#include <vfat/vfat_defs.hpp>

static std::pmr::memory_resource* G_allocator { nullptr };

namespace vfsio
{
	auto vfat_volume::set_allocator(std::pmr::memory_resource* allocator_v) -> void { G_allocator = allocator_v; }	
	static auto get_allocator() -> std::pmr::memory_resource* { return G_allocator ? G_allocator : std::pmr::get_default_resource(); }
}
	
namespace vfsio::vfat
{

	template <std::size_t Block_size>
	struct block_loader
	{
		block_loader(Iblock& block_v, std::pmr::memory_resource* allocator_v = nullptr)
		: m_allocator(allocator_v ? allocator_v : get_allocator())
		,	m_block(block_v)		
		{}

		auto read(std::uintmax_t block_start_v, std::size_t block_count_v) const 
			-> std::optional<memory::buffer<std::byte>>
		{
			auto const count_v = block_count_v * Block_size;
			auto const start_v = block_start_v * Block_size;
			
			auto buffer_v = memory::buffer<std::byte>(*m_allocator, count_v);
			
			auto const read_bytes_v = m_block.read(buffer_v, start_v);
			
			if (read_bytes_v != buffer_v.size())
				return std::nullopt;
			
			return buffer_v;
		}
				
	private:
		std::pmr::memory_resource* m_allocator{ get_allocator() };
		Iblock& m_block;	

	};
			
	static auto determine_variant(memory::buffer<std::byte> const& boot_s) 
		-> std::optional<vfsio::vfat_volume::variant_t>
	{	
		if (boot_s.size() < 512)
			return std::nullopt;
		
		smallfat_bpb const& smallfat_bpb_v = boot_s.as<const smallfat_bpb>();
		largefat_bpb const& largefat_bpb_v = boot_s.as<const largefat_bpb>();

		return std::nullopt;
		
	}

}

namespace vfsio
{
	auto vfat_volume::format([[maybe_unused]] Iblock& block_v, [[maybe_unused]] params const& params_v) -> std::tuple<error, std::unique_ptr<vfat_volume>>
	{ 
	  return { error::not_implemented, nullptr };
	}
	
	auto vfat_volume::mount([[maybe_unused]] Iblock& block_v, [[maybe_unused]] variant_t variant_v) -> std::tuple<error, std::unique_ptr<vfat_volume>>
	{
		vfat::block_loader<0x200u> loader (block_v);
		auto boot_block_o = loader.read(0, 1);
		if (!boot_block_o.has_value()) {
			return { block_v.last_error(), nullptr};
		}
		
		auto const variant_o = vfat::determine_variant(*boot_block_o);
		if (!variant_o.has_value())
		{
			return { error::bad_volume, nullptr };
		}

	  return { error::not_implemented, nullptr }; 
	}
}
