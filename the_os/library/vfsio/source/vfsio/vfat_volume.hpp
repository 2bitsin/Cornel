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
  struct vfat_volume: public Ivolume
  {   
    enum class variant_t : int
    {
      _default = 0,
      fat12 = 12,
      fat16 = 16,
      fat32 = 32
    };

    struct params
    { 
      params()
      : which_variant { variant_t::_default }
      , unit_size     { 512 }
      , full_format   { false }
      {}  

      variant_t     which_variant ;
      std::uint32_t unit_size     ;
      bool          full_format   ;
    };
    
    static auto set_allocator(std::pmr::memory_resource* allocator_v) -> void;
  
    static auto format (Iblock& block_v, params const& params_v = params())->std::tuple<error, std::unique_ptr<vfat_volume>>;
    static auto mount (Iblock& block_v, variant_t variant_v = variant_t::_default) -> std::tuple<error, std::unique_ptr<vfat_volume>>;

    virtual auto label() const -> std::string_view = 0;
    virtual auto bytes_free() const -> std::size_t = 0;

  protected:
    vfat_volume() = default;
  };
}