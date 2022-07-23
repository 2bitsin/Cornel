#include <limits>
#include <array>

#include "packet_io_pipe.hpp"
#include "cobs_encoder.hpp"
#include "error.hpp"
#include "common/crc32.hpp"

packet_io_pipe::packet_io_pipe(std::string_view name, client_vs_server_type cvs)
: m_handle{ INVALID_HANDLE_VALUE }
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

void packet_io_pipe::send(packet_buffer<byte> const& packet)
{
  if (packet.size() < 1)
    throw error_invalid_parameter(format_error, "packet size");
  
  if (packet.size() > 0x10000)
  {
    for (auto&& packet : packet.split(0x10000))    
      send(packet);
    return;
  } 

	cobs_encoder encoder;

	encoder.init();
	encoder.write<std::uint32_t>('!GSM');
	encoder.write<std::uint16_t>(packet.size());
	encoder.write(packet.data(), packet.size());
	encoder.write<std::uint32_t>(packet.crc32());
	encoder.done();
	
	send_bytes(encoder.data(), encoder.size());	
}

auto packet_io_pipe::recv() -> packet_buffer<byte>
{
  return { 1 };
}

auto packet_io_pipe::send_bytes(const byte* bytes, std::size_t size) -> std::size_t
{
  unsigned long nWritten{ 0 };
  if constexpr (sizeof(size) > sizeof(DWORD)) {
    if (size > std::numeric_limits<DWORD>::max())
      throw error_buffer_too_big(format_error);
  }
  if (!WriteFile(m_handle, bytes, (unsigned long)size, &nWritten, nullptr))
    throw error_cant_write_pipe(format_error);
  return nWritten;  
}

auto packet_io_pipe::recv_bytes(byte* byte, std::size_t size) -> std::size_t
{
  unsigned long nRead{ 0 };
  if constexpr (sizeof(size) > sizeof(DWORD)) {
    if (size > std::numeric_limits<DWORD>::max())
      size = std::numeric_limits<DWORD>::max();
  }
  if (!ReadFile(m_handle, byte, (unsigned long)size, &nRead, nullptr))
    throw error_cant_read_pipe(format_error);
  return nRead;
}
