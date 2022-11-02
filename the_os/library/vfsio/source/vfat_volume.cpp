#include <vfsio/vfat_volume.hpp>
#include <memory/buffer.hpp>

#include <algorithm>

static std::pmr::memory_resource* G_allocator { nullptr };

auto vfsio::vfat_volume::set_allocator(std::pmr::memory_resource* allocator_v) -> void
{
  G_allocator = allocator_v;
}

static auto get_allocator() -> std::pmr::memory_resource*
{
  return G_allocator ? G_allocator : std::pmr::get_default_resource();
}

[[maybe_unused]] static auto determine_variant([[maybe_unused]] vfsio::Iblock& block_v) -> vfsio::vfat_volume::variant_t
{
  using namespace vfsio;

  auto const block_aligment_v = std::max<std::size_t>(block_v.aligment(), 512u);
  
  auto sector_buffer_v = memory::buffer<std::byte>{ 
    *get_allocator(), 
    block_aligment_v
  };

  return vfat_volume::variant_t::_default;
}

auto vfsio::vfat_volume::format([[maybe_unused]] Iblock& block_v, [[maybe_unused]] params const& params_v) -> std::tuple<error, std::unique_ptr<vfat_volume>>
{ 
  return { error::not_implemented, nullptr };
}

auto vfsio::vfat_volume::mount([[maybe_unused]] Iblock& block_v, [[maybe_unused]] variant_t variant_v) -> std::tuple<error, std::unique_ptr<vfat_volume>>
{
  return { error::not_implemented, nullptr }; 
}

