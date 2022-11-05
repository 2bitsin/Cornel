#include <cstring>
#include <cassert>
#include <algorithm>
#include <optional>
#include <memory>
#include <new>
#include <bit>

#include <textio/logger.hpp>
#include <memory/buffer.hpp>

#include <vfsio/vfat_volume.hpp>
#include <vfsio/idirectory.hpp>
#include <vfat/defs.hpp>
#include <vfat/utils.hpp>

namespace vfsio::vfat
{
  declare_module(VFAT);
      
  struct volume_impl: 
    public vfsio::vfat_volume
  {
    
    Iblock* m_block { nullptr };

    union
    {
      std::byte bytes[512];
      small_bpb small;
      large_bpb large;
    } m_bpb;

    vfsio::vfat_variant m_variant ;
    
    std::uint32_t m_root_sectors ;
    std::uint32_t m_fat_sectors ;
    std::uint32_t m_total_sectors ;
    std::uint32_t m_data_sectors ;
    std::uint32_t m_data_clusters ;
    
    auto initialize(Iblock& block_v) -> error         
    {       
      clear_error();
      
      if (block_v.read(m_bpb.bytes, 0u) < sizeof(m_bpb.bytes)) {
        return error::bad_volume;
      }
      
      auto const& small_v = m_bpb.small;
      auto const& large_v = m_bpb.large;
      
      /////////////////////////////////
      //
      //  VALIDATE BIOS PARAMETER BLOCK
      //
      /////////////////////////////////
            
      if (small_v.Signature != vfat::fat_bs_signature) {
        Gmod.debug<"Signature is not 0xAA55.">();
        return error::bad_volume;
      }

      if (!std::has_single_bit(small_v.BPB_BytsPerSec) || small_v.BPB_BytsPerSec < 512u || small_v.BPB_BytsPerSec > 4096u) {
        Gmod.debug<"BPB_BytsPerSec is not a positive power of 2, between 512 and 4096">();
        return error::bad_volume;
      }
      
      if (!std::has_single_bit(small_v.BPB_SecPerClus) || small_v.BPB_SecPerClus < 1u) {
        Gmod.debug<"BPB_SecPerClus is not a positive power of 2, betweem 1 and 128">();
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

      /////////////////////////////////////////////////////
      //
      //  CALCULATE VOLUME PARAMETERS AND DETERMINE VARIANT
      //
      /////////////////////////////////////////////////////
            
      m_root_sectors  = (small_v.BPB_RootEntCnt * 32u + (small_v.BPB_BytsPerSec - 1u)) / small_v.BPB_BytsPerSec;
      m_fat_sectors   = first_true_value(small_v.BPB_FATSz16, large_v.BPB_FATSz32);   
      m_total_sectors = first_true_value(small_v.BPB_TotSec16, small_v.BPB_TotSec32);
      m_data_sectors  = m_total_sectors - (small_v.BPB_RsvdSecCnt + small_v.BPB_NumFATs*m_fat_sectors + m_root_sectors);
      m_data_clusters = m_data_sectors / small_v.BPB_SecPerClus;

      if (m_data_sectors >= m_total_sectors) {
        Gmod.debug<"BPB_TotSec16 and/or BPB_TotSec32 is invalid.">();
        return error::bad_volume;
      }
            
      m_variant = ([](auto value)
      {     
        if (value < 4085u) 
          return vfsio::vfat_variant::fat12;        
        else if (value < 65525u) 
          return vfsio::vfat_variant::fat16;
        else 
          return vfsio::vfat_variant::fat32;        
        
      }) (m_data_clusters);
      
      ////////////////////////////////

      m_block = &block_v;

      return error::success;
    }
        
    auto read_sectors(std::uintmax_t offset_v, std::size_t count_v) const
      -> std::tuple<error, memory::buffer<std::byte>>
    {
      clear_error();

      if (nullptr == m_block) {
        Gmod.debug<"Volume not initialized.">();
        set_error(error::invalid_handle);
        return { error::invalid_handle, {} };
      }
      
      offset_v *= m_bpb.small.BPB_BytsPerSec;
      count_v *= m_bpb.small.BPB_BytsPerSec;

      memory::buffer<std::byte> buffer_v (count_v);
      
      if (m_block->read(buffer_v, offset_v) < count_v) {
        Gmod.debug<"Failed to read from block.">();
        set_error(m_block->last_error());
        return { m_block->last_error(), {} };
      }

      return { error::success, std::move (buffer_v) };
    }


    template <vfat_variant Variant>
    auto next_cluster_index_impl(std::uint32_t cluster_v) const -> std::uint32_t
    {
      clear_error();
      [[maybe_unused]] auto const& small_v = m_bpb.small;
      [[maybe_unused]] auto const& large_v = m_bpb.large;      

      auto const offset_v = ([] (auto cluster_v) -> std::uintmax_t {
        if constexpr (Variant == vfat_variant::fat12)
          return cluster_v + (cluster_v / 2u);
        else if constexpr (Variant == vfat_variant::fat16)
          return cluster_v * 2u;
        else if constexpr (Variant == vfat_variant::fat32)
          return cluster_v * 4u;
        else {
          static_assert(sizeof(cluster_v)==0, "Invalid variant.");
        }
      })(cluster_v);

      auto const sector_v = small_v.BPB_RsvdSecCnt + (offset_v / small_v.BPB_BytsPerSec);
      auto const offset_in_sector_v = offset_v % small_v.BPB_BytsPerSec;
      auto const [error_v, buffer_v] = read_sectors(sector_v, (Variant == vfat_variant::fat12 ? 2u : 1u));
      if (error_v != error::success) {
        set_error(error_v);
        return 0xFFFFFFFFu;
      }
      
      if constexpr (Variant == vfat_variant::fat32)
        return *(std::uint32_t const*)(buffer_v.data() + offset_in_sector_v) & 0x0FFFFFFFu;
      else if constexpr (Variant == vfat_variant::fat16)
        return *(std::uint16_t const*)(buffer_v.data() + offset_in_sector_v);
      else if constexpr (Variant == vfat_variant::fat12) {        
        auto const value = *(std::uint16_t const*)(buffer_v.data() + offset_in_sector_v);
        return cluster_v & 1 ? value >> 4u : value & 0x0FFFu;
      }
      else
      {
        Gmod.debug<"Invalid variant.">();
        set_error(error::bad_volume);
        return 0xFFFFFFFFu;
      }
    }

    auto next_cluster_index(std::uint32_t cluster_v) const -> std::uint32_t
    {
      clear_error();
      [[maybe_unused]] auto const& small_v = m_bpb.small;
      [[maybe_unused]] auto const& large_v = m_bpb.large;

      switch (m_variant)
      {
      using enum vfat_variant;
      case fat12: return next_cluster_index_impl<vfat_variant::fat12>(cluster_v);       
      case fat16: return next_cluster_index_impl<vfat_variant::fat16>(cluster_v);
      case fat32: return next_cluster_index_impl<vfat_variant::fat32>(cluster_v);
      default:    
        Gmod.debug<"Invalid variant.">();
        set_error(error::bad_volume);
        return 0xFFFFFFFFu;
      }
    }

    auto const& small_bpb() const { return m_bpb.small; }
    auto const& large_bpb() const { return m_bpb.large; }

    auto open(std::string_view path_v, std::string_view mode_v = "") -> std::unique_ptr<Iblock> override; 
  };  

  
  struct small_directory_impl: 
    public Idirectory
  {
    small_directory_impl(volume_impl& volume_v, std::uintmax_t first_v, std::uintmax_t count_v)
    : m_volume (volume_v)
    , m_first  (first_v)
    , m_count  (count_v)
    , m_sector (first_v)
    , m_offset (0)
    , m_buffer ()
    , m_current {}
    {
      clear_error();
      auto [error_v, buffer_v] = volume_v.read_sectors(first_v, 1u);
      if (error::success != error_v) {
        set_error(error_v);
        return;
      }
      m_buffer = std::move(buffer_v);
    }           
    
    auto read(std::span<std::byte> buffer_v) -> std::size_t
    {     
      clear_error();
      const auto entry_count_v =  m_volume.small_bpb().BPB_BytsPerSec / 32u;      
      while (true)
      { 
        if (m_offset >= entry_count_v)
        {
          m_sector += 1u;
          m_offset = 0u;
          auto [error_v, buffer_v] = m_volume.read_sectors(m_sector, 1u);
          if (error::success != error_v) {
            set_error(m_volume.last_error());
            return 0u;
          }
          m_buffer = std::move (buffer_v);
          m_offset = 0u;          
        }       
        auto entries_v = m_buffer.as_array<const directory_entry>();
        std::memcpy(&m_current, &entries_v[m_offset], sizeof(directory_entry));
        ++m_offset;
        if (m_current.DIR_Name[0] == 0xE5u)
          continue;
        if (m_current.DIR_Name[0] == 0x00u)
          return 0u;        
        return copy_name(buffer_v, m_current.DIR_Name);
      }     
    }
    
    auto copy_name(std::span<std::byte> buffer_v, std::uint8_t (&name_v) [11u]) const -> std::size_t
    {
      auto i = 0u;
      while (name_v[i] != ' ' && i < 8u && i < buffer_v.size()) {
        buffer_v[i] = (std::byte)name_v[i];
        ++i;
      }
      
      if (name_v[8] != ' ' && i < buffer_v.size())
      {
        buffer_v[i++] = (std::byte)'.';
        auto j = 0u;
        while (name_v[8 + j] != ' ' && j < 3u && i < buffer_v.size())
          buffer_v[i++] = (std::byte)name_v[8 + j++];
      }     
      return i;
    }

  private:
    volume_impl &  m_volume;
    std::uintmax_t m_first;
    std::uintmax_t m_count;
    std::uintmax_t m_sector;
    std::uintmax_t m_offset;    
    memory::buffer<std::byte> m_buffer;
    directory_entry m_current;
  };

  auto volume_impl::open(std::string_view path_v, std::string_view mode_v) -> std::unique_ptr<Iblock>
  {
    clear_error();
    [[maybe_unused]] const auto& small_v = m_bpb.small;
    [[maybe_unused]] const auto& large_v = m_bpb.large;
    if (path_v == "/" || path_v.empty()) 
    {
      if (m_variant != vfat_variant::fat32)
      {
        const auto first_v = small_v.BPB_RsvdSecCnt + (small_v.BPB_NumFATs * small_v.BPB_FATSz16);
        const auto count_v = small_v.BPB_RootEntCnt * 32u / small_v.BPB_BytsPerSec;
        return std::make_unique<small_directory_impl>(*this, first_v, count_v);
      }     
    }
    set_error(error::not_implemented);
    return nullptr;
  } 

}

namespace vfsio
{ 
  auto vfat_volume::format([[maybe_unused]] Iblock& block_v, [[maybe_unused]] params const& params_v) -> std::tuple<error, std::unique_ptr<vfat_volume>>
  { 
    return { error::not_implemented, nullptr };
  }
  
  auto vfat_volume::mount([[maybe_unused]] Iblock& block_v, [[maybe_unused]] vfat_variant variant_v) -> std::tuple<error, std::unique_ptr<vfat_volume>>
  {
    auto volume_p = std::make_unique<vfat::volume_impl>();
    if (const auto error_v = volume_p->initialize(block_v); error_v != error::success) 
      return { error_v, nullptr };
    return { error::success, std::move(volume_p) };     
  }
}
