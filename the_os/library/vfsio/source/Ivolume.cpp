
#include <vfsio/Ivolume.hpp>

using vfsio::Ivolume;

auto Ivolume::open([[maybe_unused]] std::string_view path_v, [[maybe_unused]] std::string_view mode_v) -> std::unique_ptr<Iblock>
{
  set_error(error::not_implemented);
  return nullptr;
}

auto Ivolume::exists([[maybe_unused]] std::string_view path_v) -> bool
{
  return !!open(path_v, "");
}


auto Ivolume::remove([[maybe_unused]] std::string_view path_v, [[maybe_unused]] bool recursive_v) -> bool
{
  set_error(error::not_implemented);
  return false;
}

auto Ivolume::rename_or_move([[maybe_unused]] std::string_view old_path_v, [[maybe_unused]] std::string_view new_path_v) -> bool
{
  set_error(error::not_implemented);
  return false; 
}

auto Ivolume::make_directory([[maybe_unused]] std::string_view path_v, [[maybe_unused]] bool recursive_v) -> bool
{
  set_error(error::not_implemented);
  return false;   
}
