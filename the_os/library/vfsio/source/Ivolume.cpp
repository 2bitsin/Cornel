#pragma once

#include <vfsio/Ivolume.hpp>

using vfsio::Ivolume;

auto Ivolume::open(std::string_view, std::string_view) -> std::unique_ptr<Iblock>
{
	set_error(error::not_implemented);
	return nullptr;
}

auto Ivolume::exists(std::string_view path_v) -> bool
{
	return !!open(path_v, "");
}


auto Ivolume::remove(std::string_view path_v, bool recursive_v) -> bool
{
	set_error(error::not_implemented);
	return false;
}

auto Ivolume::rename_or_move(std::string_view old_path_v, std::string_view new_path_v) -> bool
{
	set_error(error::not_implemented);
	return false;	
}

auto Ivolume::make_directory(std::string_view path_v, bool recursive_v) -> bool
{
	set_error(error::not_implemented);
	return false;		
}
