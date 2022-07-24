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
		/* 12 */ "Can't write pipe.",
		/* 13 */ "Can't read pipe.",
		/* 14 */ "Buffer too large for write or read operation.",
		/* 15 */ "Can't send packet header.",
		/* 16 */ "Can't send packet data.",
		/* 17 */ "Can't receive packet header.",
		/* 18 */ "Can't receive packet data.",
		/* 19 */ "Invalid header signature.",
		/* 20 */ "Invalid packet checksum.",
		/* 21 */ "Failed to decode packet correctly.",
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
using error_buffer_too_big    = generic_error_t<14, errtab_t>;
using error_cant_send_header	= generic_error_t<15, errtab_t>;
using error_cant_send_payload = generic_error_t<16, errtab_t>;
using error_cant_read_header  =	generic_error_t<17, errtab_t>;
using error_cant_read_payload =	generic_error_t<18, errtab_t>;
using error_invalid_header		= generic_error_t<19, errtab_t>;
using error_invalid_checksum	= generic_error_t<20, errtab_t>;
using error_decode_failed			= generic_error_t<21, errtab_t>;