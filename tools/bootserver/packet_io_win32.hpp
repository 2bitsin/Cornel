#pragma once

#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include "packet_io.hpp"

struct packet_io_win32
:	public Ipacket_io,
	public packet_io_connect
{
	struct pipe_t {};
	struct serial_t {};
	
	static inline constexpr const serial_t use_serial;
	static inline constexpr const pipe_t use_pipe;

	packet_io_win32(pipe_t, std::string_view name, client_vs_server_type cvs);
 ~packet_io_win32();

	packet_io_win32(packet_io_win32 const&) = delete;
	packet_io_win32(packet_io_win32 &&) = delete;
	packet_io_win32& operator=(packet_io_win32 const&) = delete;
	packet_io_win32& operator=(packet_io_win32 &&) = delete;
	
	void send(packet_buffer<byte> const&) override;
	auto recv() -> packet_buffer<byte> override;
protected:
	auto send_bytes(const byte* bytes, std::size_t size) -> std::size_t;
	auto recv_bytes(byte* byte, std::size_t size) -> std::size_t;

private:
	HANDLE m_handle { nullptr };
};