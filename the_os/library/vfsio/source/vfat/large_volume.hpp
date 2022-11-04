#pragma once

#include <vfat/defs.hpp>
#include <vfsio/ibase.hpp>
#include <vfsio/iblock.hpp>
#include <vfsio/vfat_volume.hpp>

namespace vfsio::vfat
{
	template<vfat_volume::variant_t Variant>
	struct large_volume: 
		public vfsio::vfat_volume
	{
		static_assert(Variant != vfat_volume::variant_t::_default, "Variant must be specified");		
		
		large_volume(Iblock& block_v, 
			largefat_bpb const& bpb_v, 
			std::pmr::memory_resource* alloc_v)
		: m_alloc { alloc_v }
		,	m_block { block_v }
		, m_bpb   { bpb_v   }
		{ }

		auto label() const -> std::string_view override { return ""; };
    auto bytes_free() const -> std::size_t override { return 0u; };

	private:
		std::pmr::memory_resource* m_alloc;
		Iblock& m_block;
		largefat_bpb m_bpb;
	};
}