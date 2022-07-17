#pragma once

#include "common/generic_error.hpp"

template <unsigned int Error_code>
struct errtab_t
{		
	constexpr inline static const std::string_view errtab [] =
	{
		/*  0 */ "Operation completed successfully.",
		/*  1 */ "Couldn't get next byte, stream returned : {}.",
		/*  2 */ "Bad or unrecognized command token : {}.",
		/*  3 */ "The path '{}' does not exist.", 
		/*  4 */ "The path '{}' is not a direcotory.",
		/*  5 */ "The path '{}' is not a file.",
		/*  6 */ "Bad command format : '{}'.",
		/*  7 */ "Method or function '{}' not implemented.",
		/*  8 */ "Invalid parameter value for '{}'.",
		/*  9 */ "Can't create named pipe '{}'.",
		/* 10 */ "Can't open named pipe '{}'.",
		/* 11 */ "Can't connect named pipe '{}'.",
		/* 12 */ "Can't write named pipe '{}'.",
	};

	static constexpr auto value()
	{
		static_assert (Error_code < std::size(errtab));
		return errtab [Error_code];
	}
};

using error_success						= generic_error_t< 0, errtab_t>;
using error_bad_stream				= generic_error_t< 1, errtab_t>;
using error_bad_command				= generic_error_t< 2, errtab_t>;
using error_does_not_exist		= generic_error_t< 3, errtab_t>;
using error_bad_directory			= generic_error_t< 4, errtab_t>;
using error_bad_file					= generic_error_t< 5, errtab_t>;
using error_bad_format				= generic_error_t< 6, errtab_t>;
using error_not_implemented		= generic_error_t< 7, errtab_t>;
using error_invalid_parameter = generic_error_t< 8, errtab_t>;
using error_cant_create_pipe	= generic_error_t< 9, errtab_t>;
using error_cant_open_pipe		= generic_error_t<10, errtab_t>;
using error_cant_connect_pipe	= generic_error_t<11, errtab_t>;
using error_cant_write_pipe		= generic_error_t<12, errtab_t>;
using error_cant_read_pipe		= generic_error_t<13, errtab_t>;
