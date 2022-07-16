#pragma once

#include "common/generic_error.hpp"

template <unsigned int Error_code>
struct errtab_t
{		
	constexpr inline static const std::string_view errtab [] =
	{
		"Operation completed successfully.",
		"Couldn't get next byte, stream returned : {}.",
		"Bad or unrecognized command token : {}.",
		"The path '{}' does not exist.", 
		"The path '{}' is not a direcotory.",
		"The path '{}' is not a file.",
		"Bad command format : '{}'."
	};

	static constexpr auto value()
	{
		static_assert (Error_code < std::size(errtab));
		return errtab [Error_code];
	}
};

using error_success					= generic_error_t<0, errtab_t>;
using error_bad_stream			= generic_error_t<1, errtab_t>;
using error_bad_command			= generic_error_t<2, errtab_t>;
using error_does_not_exist	= generic_error_t<3, errtab_t>;
using error_bad_directory		= generic_error_t<4, errtab_t>;
using error_bad_file				= generic_error_t<5, errtab_t>;
using error_bad_format			= generic_error_t<6, errtab_t>;
