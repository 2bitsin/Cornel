#pragma once

#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include "packet_io.hpp"

struct packet_io_win32
:	public packet_io_interface,
	public packet_io_connect
{
	struct serial_t {};
	struct pipe_t {};
	
	static inline constexpr const serial_t use_serial {};
	static inline constexpr const pipe_t use_pipe {};

	packet_io_win32(pipe_t, std::string_view name, client_versus_server_type cvs);
	packet_io_win32(serial_t, std::string_view name, serial_params_type const& params);
 ~packet_io_win32();

	packet_io_win32(packet_io_win32 const&) = delete;
	packet_io_win32(packet_io_win32 &&) = delete;
	packet_io_win32& operator=(packet_io_win32 const&) = delete;
	packet_io_win32& operator=(packet_io_win32 &&) = delete;
	
	void send(packet_buffer<byte> const&) override;
	auto recv() -> packet_buffer<byte> override;
		
protected:
	static inline constexpr const uint16_t PACKET_SIGNATURE = 0x55AA;

	static void init_comm_state(std::string_view name, HANDLE handle, serial_params_type const&);
	
	auto send_bytes(const byte* bytes, std::size_t size) -> std::size_t;
	auto recv_bytes(byte* byte, std::size_t size) -> std::size_t;

private:
	HANDLE m_handle { nullptr };
};