#include <vfsio/vfat_volume.hpp>
#include <memory/buffer.hpp>
#include <textio/logger.hpp>

#include <cassert>
#include <algorithm>
#include <optional>

#include <vfat/defs.hpp>
#include <vfat/utils.hpp>
#include <vfat/block_loader.hpp>

	
namespace vfsio::vfat
{
	declare_module(VFAT);

	struct context
	{
		std::byte boot_sector[512];
		vfsio::vfat_variant variant ;		
		std::uint32_t root_sectors ;
		std::uint32_t fat_sectors ;
		std::uint32_t total_sectors ;
		std::uint32_t data_sectors ;
		std::uint32_t data_clusters ;
		
		auto initialize(memory::buffer<std::byte> const& boot_s) 
			-> vfsio::error		
		{	
			if (boot_s.size() < 512)
				return error::bad_volume;
			
			auto const& small_v = boot_s.as<const small_bpb>();
			auto const& large_v = boot_s.as<const large_bpb>();
			
			if (!small_v.BPB_BytsPerSec) {
				Gmod.debug<"BPB_BytsPerSec is 0.">();
				return error::bad_volume;
			}
			
			if (!small_v.BPB_SecPerClus) {
				Gmod.debug<"BPB_SecPerClus is 0.">();
				return error::bad_volume;
			}

			if (!small_v.BPB_FATSz16 && !large_v.BPB_FATSz32) {
				Gmod.debug<"Both BPB_FATSz16 and BPB_FATSz32 are 0.">();
				return error::bad_volume;
			}
			
			if (small_v.BPB_NumFATs < 1u || small_v.BPB_NumFATs > 2u) {
				Gmod.debug<"BPB_NumFATs must be between 1 and 2.">();
				return error::bad_volume;
			}

			if (!small_v.BPB_RsvdSecCnt) {
				Gmod.debug<"BPB_RsvdSecCnt is 0.">();			
				return error::bad_volume;
			}
			
			if (small_v.Signature != vfat::fat_bs_signature) {
				Gmod.debug<"Signature is not 0xAA55.">();
				return error::bad_volume;
			}								

			root_sectors = (small_v.BPB_RootEntCnt * 32u + (small_v.BPB_BytsPerSec - 1u)) / small_v.BPB_BytsPerSec;
			fat_sectors = first_true_value(small_v.BPB_FATSz16, large_v.BPB_FATSz32);		
			total_sectors = first_true_value(small_v.BPB_TotSec16, small_v.BPB_TotSec32);
			data_sectors = total_sectors - (small_v.BPB_RsvdSecCnt + small_v.BPB_NumFATs*fat_sectors + root_sectors);
			data_clusters = data_sectors / small_v.BPB_SecPerClus;

			if (data_sectors >= total_sectors) {
				Gmod.debug<"BPB_TotSec16 and/or BPB_TotSec32 is invalid.">();
				return error::bad_volume;
			}
			
			std::memcpy(boot_sector, boot_s.data(), 
				std::min(boot_s.size(), sizeof(boot_sector)));

			variant = vfsio::vfat_variant::_default;			
			if (data_clusters < 4085 ) {			
				variant = vfsio::vfat_variant::fat12;
			}
			else if (data_clusters < 65525) {
				variant = vfsio::vfat_variant::fat16;
			}		
			else {
				variant = vfsio::vfat_variant::fat32;				
			}
			
			return error::success;
		}
	};	

	template <vfsio::vfat_variant Variant>
	struct volume_impl : public vfsio::vfat_volume
	{
		using bpb_type = std::conditional_t<Variant != vfat_variant::fat32, vfat::small_bpb,  vfat::large_bpb>;
			
		volume_impl(Iblock& block_v, context const& context_v, std::pmr::memory_resource* alloc_v = nullptr)
		: m_alloc	        { alloc_v ? alloc_v : std::pmr::get_default_resource() }
		, m_block	        { block_v }
		, m_root_sectors  { context_v.root_sectors  }
		, m_fat_sectors   { context_v.fat_sectors   }
		, m_total_sectors { context_v.total_sectors }
		, m_data_sectors  { context_v.data_sectors  }
		, m_data_clusters { context_v.data_clusters }
		{
			
		}

	private:
		std::pmr::memory_resource* m_alloc;
		Iblock& m_block;

		std::uint32_t m_root_sectors ;
		std::uint32_t m_fat_sectors ;
		std::uint32_t m_total_sectors ;
		std::uint32_t m_data_sectors ;
		std::uint32_t m_data_clusters ;

	};	
}

namespace vfsio
{	
	auto vfat_volume::format([[maybe_unused]] Iblock& block_v, [[maybe_unused]] params const& params_v) -> std::tuple<error, std::unique_ptr<vfat_volume>>
	{ 
	  return { error::not_implemented, nullptr };
	}
	
	auto vfat_volume::mount([[maybe_unused]] Iblock& block_v, [[maybe_unused]] vfat_variant variant_v) -> std::tuple<error, std::unique_ptr<vfat_volume>>
	{
		using namespace vfat;

		block_loader<0x200u> loader (block_v);
		auto boot_o = loader.load(0, 1);
		if (!boot_o.has_value()) {
			return { block_v.last_error(), nullptr};
		}
		
		auto context_p = std::make_unique<context>();
		if (error::success != context_p->initialize(*boot_o)) {
			Gmod.error<"Unable to mount volume, BPB corrupted.">();
			return { error::bad_volume, nullptr };
		}
		
		std::unique_ptr<vfat_volume> volume_p;
		switch(context_p->variant)
		{
		using enum vfat_variant;
		case fat12: volume_p.reset(new volume_impl<fat12> (block_v, *context_p)); break;
		case fat16: volume_p.reset(new volume_impl<fat16> (block_v, *context_p)); break;
		case fat32: volume_p.reset(new volume_impl<fat32> (block_v, *context_p)); break;
		}

		const auto error_v = volume_p->last_error();
		return { error_v, std::move(volume_p) };
	}
}
