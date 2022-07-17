#include "packet_io_pipe.hpp"
#include "common/crc32.hpp"
#include "error.hpp"

packet_io_pipe::packet_io_pipe(std::string_view name, client_vs_server_type cvs)
:	m_handle{ INVALID_HANDLE_VALUE }
{
	HANDLE handle{ INVALID_HANDLE_VALUE };
	if (cvs == as_client)
	{
		handle = CreateFile(name.data(), GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		if (handle == INVALID_HANDLE_VALUE || handle == nullptr)
			throw error_cant_open_pipe(format_error, name);
	}
	else if (cvs == as_server)
	{
		handle = CreateNamedPipeA(name.data(), PIPE_ACCESS_DUPLEX|FILE_FLAG_FIRST_PIPE_INSTANCE, PIPE_TYPE_BYTE|PIPE_WAIT, 1, 0x1000, 0x1000, 0, nullptr);
		if (handle == INVALID_HANDLE_VALUE || handle == nullptr)
			throw error_cant_create_pipe(format_error, name);
		if (!ConnectNamedPipe(handle, nullptr))
			throw error_cant_connect_pipe(format_error, name);
	}
	else
	{
		throw error_invalid_parameter(format_error, "client_vs_server");
	}
	m_handle = handle;
}

packet_io_pipe::~packet_io_pipe()
{
	if(m_handle != INVALID_HANDLE_VALUE && m_handle != nullptr)
		CloseHandle(m_handle);
}

void packet_io_pipe::send(packet_buffer<byte> const& Qpacket)
{
	if (Qpacket.size() > 0x10000)
	{
		for (auto&& packet : Qpacket.split(0x10000))		
			send(packet);
		return;
	}
	
}

auto packet_io_pipe::recv() -> packet_buffer<byte>
{
	return packet_buffer<byte>(0);
}
