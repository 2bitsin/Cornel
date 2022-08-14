#pragma once

#include <cstdint>
#include <cstddef>
#include <iostream>


#include "tftp_packet.hpp"

struct tftp_block_reader
{	
	tftp_block_reader(std::istream& stream);
	
	auto stream_length(std::uintmax_t value) -> tftp_block_reader&;
	auto initial_block_number(std::uintmax_t value) -> tftp_block_reader&;
	auto rollover_block_number(std::uintmax_t value) -> tftp_block_reader&;

	auto read_block(std::uintmax_t block_number) -> tftp_packet;
	
		
private:
	std::istream&  m_stream;
	std::uintmax_t m_length;

	std::uintmax_t m_remaining_length;
	std::uintmax_t m_current_block_number;
	std::uintmax_t m_initial_block_number;
	std::uintmax_t m_rollover_block_number;
};