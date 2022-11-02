#pragma once

#include <memory>
#include <string_view>
#include <string>

#include <vfsio/error.hpp>
#include <vfsio/ibase.hpp>
#include <vfsio/iblock.hpp>

namespace vfsio
{
	using namespace std::string_literals;
	
	static inline const auto mode_write				= "w"s;
	static inline const auto mode_read				= "r"s;
	static inline const auto mode_append			= "a"s;
	static inline const auto mode_open_only		= "o"s; // exists: open;  !exists: fail
	static inline const auto mode_create_only = "c"s;	// exists: fail;  !exists: create
	static inline const auto mode_truncate		= "t"s;	// exists: erase;	!exists: create

	
	
  struct Ivolume: public Ibase
  {
    virtual ~Ivolume() = default;
		
    virtual auto open (std::string_view path_v, std::string_view mode_v = "") -> std::unique_ptr<Iblock>;
		virtual auto create (std::string_view path_v, std::size_t size_v) -> std::unique_ptr<Iblock>;		
    virtual auto exists (std::string_view path_v) -> bool;
		virtual auto remove (std::string_view path_v, bool recursive_v = false) -> bool;
		virtual auto rename_or_move	(std::string_view old_path_v, std::string_view new_path_v) -> bool;	
		virtual auto make_directory (std::string_view path_v, bool recursive_v = true) -> bool;
		
  };
}