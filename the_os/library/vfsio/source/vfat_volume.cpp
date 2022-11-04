#include <vfsio/vfat_volume.hpp>
#include <memory/buffer.hpp>
#include <textio/logger.hpp>

#include <cassert>
#include <algorithm>
#include <optional>

#include <vfat/defs.hpp>
#include <vfat/utils.hpp>
#include <vfat/block_loader.hpp>
#include <vfat/large_volume.hpp>
#include <vfat/small_volume.hpp>

declare_module(VFAT);

static std::pmr::memory_resource* G_allocator { nullptr };

namespace vfsio
{
	auto vfat_volume::set_allocator(std::pmr::memory_resource* allocator_v) -> void { G_allocator = allocator_v; }	
	static auto get_allocator() -> std::pmr::memory_resource* { return G_allocator ? G_allocator : std::pmr::get_default_resource(); }
}
	
namespace vfsio::vfat
{			
	static auto determine_variant(memory::buffer<std::byte> const& boot_s) 
		-> std::optional<vfsio::vfat_volume::variant_t>
	{	
		if (boot_s.size() < 512)
			return std::nullopt;
		
		auto const& small_v = boot_s.as<const smallfat_bpb>();
		auto const& large_v = boot_s.as<const largefat_bpb>();
		
		if (!small_v.BPB_BytsPerSec) {
			Gmod.debug<"BPB_BytsPerSec is 0.">();
			return std::nullopt;
		}
		
		if (!small_v.BPB_SecPerClus) {
			Gmod.debug<"BPB_SecPerClus is 0.">();
			return std::nullopt;
		}

		if (!small_v.BPB_FATSz16 && !large_v.BPB_FATSz32) {
			Gmod.debug<"Both BPB_FATSz16 and BPB_FATSz32 are 0.">();
			return std::nullopt;
		}
		
		if (small_v.BPB_NumFATs < 1u || small_v.BPB_NumFATs > 2u) {
			Gmod.debug<"BPB_NumFATs must be between 1 and 2.">();
			return std::nullopt;
		}

		if (!small_v.BPB_RsvdSecCnt) {
			Gmod.debug<"BPB_RsvdSecCnt is 0.">();			
			return std::nullopt;
		}

		const auto root_size_v = (small_v.BPB_RootEntCnt * 32u + (small_v.BPB_BytsPerSec - 1u)) / small_v.BPB_BytsPerSec;
		const auto fat_size_v = first_true_value(small_v.BPB_FATSz16, large_v.BPB_FATSz32);		
		const auto tot_size_v = first_true_value(small_v.BPB_TotSec16, small_v.BPB_TotSec32);
		const auto data_size_v = tot_size_v - (small_v.BPB_RsvdSecCnt + small_v.BPB_NumFATs*fat_size_v + root_size_v);
		const auto clusters_v = data_size_v / small_v.BPB_SecPerClus;

		if (data_size_v >= tot_size_v) {
			Gmod.debug<"BPB_TotSec16 and/or BPB_TotSec32 is invalid.">();
			return std::nullopt;
		}
		
		if (clusters_v < 4085 ) {			
			return vfat_volume::variant_t::fat12;
		}

		if (clusters_v < 65525) {
			return vfat_volume::variant_t::fat16;
		}
		
		return vfat_volume::variant_t::fat32;				
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
		vfat::block_loader<0x200u> loader (block_v, get_allocator());
		auto boot_o = loader.load(0, 1);
		if (!boot_o.has_value()) {
			return { block_v.last_error(), nullptr};
		}
		
		auto const variant_o = vfat::determine_variant(*boot_o);
		if (!variant_o.has_value()) {
			Gmod.error<"Unable to mount volume, BPB corrupted.">();
			return { error::bad_volume, nullptr };
		}
		
		std::unique_ptr<vfat_volume> volume_p { nullptr };
		switch (*variant_o)
		{
		case variant_t::fat12: 
			{
				volume_p = std::make_unique<vfat::small_volume<variant_t::fat12>>(
					block_v, (*boot_o).template as<const vfat::smallfat_bpb>(), get_allocator()) ; 
				break;
			}
		case variant_t::fat16: 
			{
				volume_p = std::make_unique<vfat::small_volume<variant_t::fat16>>(
					block_v, (*boot_o).template as<const vfat::smallfat_bpb>(), get_allocator()) ; 
				break;
			}
		case variant_t::fat32: 
			{
				volume_p = std::make_unique<vfat::large_volume<variant_t::fat32>>(
					block_v, (*boot_o).template as<const vfat::largefat_bpb>(), get_allocator()) ; 
				break;
			}
		default:
			Gmod.error<"Unsupported volume type detected.">();
			return { error::bad_volume, nullptr };
		}
		
		const auto status_v = volume_p->last_error();
		return { status_v, std::move (volume_p) };
	}
}
