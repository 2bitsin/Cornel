#pragma once
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include "packet_io.hpp"

struct packet_io_pipe
:	public Ipacket_io,
	public packet_io_connect
{
	packet_io_pipe(std::string_view name, client_vs_server_type cvs);
 ~packet_io_pipe();

	packet_io_pipe(packet_io_pipe const&) = delete;
	packet_io_pipe(packet_io_pipe &&) = delete;
	packet_io_pipe& operator=(packet_io_pipe const&) = delete;
	packet_io_pipe& operator=(packet_io_pipe &&) = delete;
	
	void send(packet_buffer<byte> const&) override;
	auto recv() -> packet_buffer<byte> override;
protected:
	auto send_bytes(const byte* bytes, std::size_t size) -> std::size_t;
	auto recv_bytes(byte* byte, std::size_t size) -> std::size_t;

private:
	HANDLE m_handle { nullptr };
};