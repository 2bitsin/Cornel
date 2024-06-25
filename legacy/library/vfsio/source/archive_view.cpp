#include <vfsio/archive_view.hpp>
#include <tuple>

using vfsio::archive_view;


auto archive_view::header_from_bytes(std::span<const std::byte> bytes_v) 
  -> header_type const*
{
  if (bytes_v.size() < offsetof(header_type, name))
    return nullptr;
  header_type const& header_v = *std::launder((header_type const*)bytes_v.data());
  if (bytes_v.size() < header_v.header_size())
    return nullptr;
  return &header_v;
}

static constexpr inline auto align(auto offset_v) 
{
  return (offset_v + 0xfu) & ~(decltype(offset_v))0xfu;
}


archive_view::archive_view(std::span<std::byte const> bytes_v)
: m_bytes(bytes_v)
{}

auto archive_view::find(error& error_v, std::string_view path_v) const -> std::tuple<std::span<std::byte const>, type_id_enum>
{
  error_v = error::none;
  return archive_view::find(error_v, path_v, m_bytes);
}

static auto path_split(std::string_view value_v) -> std::tuple<std::string_view, std::string_view>
{
  auto i = value_v.find_first_of('/');
  if (i == std::string_view::npos)
    return { value_v, {} };
  return { value_v.substr(0, i), value_v.substr(i + 1) };
}

auto archive_view::find(error& error_v, std::string_view path_v, std::span<std::byte const> bytes_v) -> std::tuple<std::span<std::byte const>, type_id_enum>
{
  error_v = error::none;
  std::size_t offset_v = 0;
  std::size_t ending_v = bytes_v.size();

  auto [first_v, rest_of_path_v] = path_split(path_v);
    
  while(offset_v < bytes_v.size())
  {
    auto const* header_p = header_from_bytes(bytes_v.subspan(offset_v));
    if (header_p == nullptr)
    {
      error_v = error::not_found;
      return {};
    }
      
    auto const& header_v = *header_p;     
    ending_v = align(offset_v + header_v.size);
    offset_v += header_v.header_size();
    
    std::string_view name_v(header_v.name, header_v.name_len);    
    if (const auto i = name_v.find_last_not_of('\0'); i != name_v.npos)
      name_v = name_v.substr(0, i+1);
    const auto sub_size_v = std::min(ending_v - offset_v, bytes_v.size() - offset_v);
    const auto sub_bytes_v = bytes_v.subspan(offset_v, sub_size_v);
    if (name_v == first_v) {
      if (rest_of_path_v.empty ()) 
        return { sub_bytes_v.subspan(0, header_v.size - header_v.header_size()), header_v.type_id };
      return find(error_v, rest_of_path_v, sub_bytes_v);
    }
    offset_v = ending_v;  
  }

  error_v = error::not_found; 
  return { std::span<std::byte const>{}, type_id_enum::undefined };
}


auto archive_view::find (error& error_v, type_id_enum type_v, std::string_view path_v) const -> std::span<std::byte const>
{
  error_v = error::none;
  auto [o_bytes_v, type_id_v] = archive_view::find(error_v, path_v);
  if (error_v != error::none)
    return {};
  if (type_id_v != type_v) {
    error_v = error::not_found;
    return {};
  }
  return o_bytes_v;
}

auto archive_view::find(error& error_v, type_id_enum type_v, std::string_view path_v, std::span<std::byte const> bytes_v) -> std::span<std::byte const>
{
  error_v = error::none;
  auto [o_bytes_v, type_id_v] = archive_view::find(error_v, path_v, bytes_v);
  if (error_v != error::none)
    return {};
  if (type_id_v != type_v) {
    error_v = error::not_found;
    return {};
  }
  return o_bytes_v;
}
