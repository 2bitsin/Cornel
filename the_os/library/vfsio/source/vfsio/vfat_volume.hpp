#pragma once

#include <cstddef>
#include <cstdint>
#include <memory_resource>

#include <tuple>
#include <memory>

#include <vfsio/error.hpp>
#include <vfsio/iblock.hpp>
#include <vfsio/ivolume.hpp>

namespace vfsio
{ 
   enum class vfat_variant : int
   {
     _default = 0,
     fat12 = 12,
     fat16 = 16,
     fat32 = 32
   };
	
  struct vfat_volume: public Ivolume
  {   

    struct params
    { 
      params()
      : which_variant { vfat_variant::_default }
      , unit_size     { 512 }
      , full_format   { false }
      {}  

      vfat_variant  which_variant ;
      std::uint32_t unit_size     ;
      bool          full_format   ;
    };
      
    static auto format (Iblock& block_v, params const& params_v = params())->std::tuple<error, std::unique_ptr<vfat_volume>>;
    static auto mount (Iblock& block_v, vfat_variant variant_v = vfat_variant::_default) -> std::tuple<error, std::unique_ptr<vfat_volume>>;

    //virtual auto volume_label() const -> std::string_view = 0;
		//virtual auto volume_id() const -> std::uint32_t = 0;
    //virtual auto bytes_free() const -> std::size_t = 0;

  protected:
    vfat_volume() = default;
  };
}