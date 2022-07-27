#include "packet_io_win32.hpp"
#include <limits>
#include <array>

#include "packet_io_win32.hpp"
#include "cobs_encoder.hpp"
#include "cobs_decoder.hpp"
#include "error.hpp"
#include "common/crc32.hpp"
#include "common/span_utility.hpp"


packet_io_win32::packet_io_win32(pipe_t, std::string_view name, client_versus_server_type cvs)
: m_handle{ INVALID_HANDLE_VALUE }
{
  HANDLE handle{ INVALID_HANDLE_VALUE };
  if (cvs == as_client)
  {
    handle = CreateFileA(name.data(), GENERIC_READ|GENERIC_WRITE, FILE_SHARE_READ|FILE_SHARE_WRITE, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
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

void packet_io_win32::init_comm_state(std::string_view name, HANDLE handle, serial_params_type const& params)
{
	DCB dcb;
	RtlSecureZeroMemory(&dcb, sizeof(dcb));
	dcb.DCBlength = sizeof(dcb);
	if (!GetCommState(handle, &dcb))
		throw error_cant_get_com_state(format_error, name);
	
	dcb.fBinary = TRUE;
	dcb.fOutxCtsFlow = FALSE;
	dcb.fOutxDsrFlow = FALSE;
	dcb.fDtrControl = DTR_CONTROL_DISABLE;
	dcb.fOutX = FALSE;
	dcb.fInX = FALSE;
	dcb.fAbortOnError = FALSE;
	
	dcb.BaudRate = params.baud_rate;
	dcb.ByteSize = params.data_bits;
	switch (params.parity)
	{
	case parity_odd:
		dcb.fParity = TRUE;
		dcb.Parity = ODDPARITY;
		break;
	case parity_even:
		dcb.fParity = TRUE;
		dcb.Parity = EVENPARITY;
		break;
	case parity_mark:
		dcb.fParity = TRUE;
		dcb.Parity = MARKPARITY;
		break;
	case parity_space:
		dcb.fParity = TRUE;
		dcb.Parity = SPACEPARITY;
		break;
	case parity_none:
	default:
		dcb.fParity = FALSE;
		dcb.Parity = NOPARITY;
		break;
	}

	switch (params.stop_bits)
	{
	case 2:
		dcb.StopBits = TWOSTOPBITS;
		break;
	default:
	case 1:
		dcb.StopBits = ONESTOPBIT;
		break;
	}

	if (!SetCommState(handle, &dcb)) {
		throw error_cant_set_com_state(format_error, name);
	}
	
}

packet_io_win32::packet_io_win32(serial_t, std::string_view name, serial_params_type const& params)
{
  HANDLE handle{ INVALID_HANDLE_VALUE };
	handle = CreateFileA(name.data(), GENERIC_READ|GENERIC_WRITE, 0, nullptr, OPEN_EXISTING, 0, nullptr);
	if (handle == INVALID_HANDLE_VALUE || handle == nullptr)
		throw error_cant_open_serial(format_error, name);
	init_comm_state(name, handle, params);
	m_handle = handle;	
}

packet_io_win32::~packet_io_win32()
{
  if(m_handle != INVALID_HANDLE_VALUE && m_handle != nullptr)
    CloseHandle(m_handle);
}

void packet_io_win32::send(packet_buffer<byte> const& packet, std::uint32_t to)
{
	using clock = std::chrono::high_resolution_clock;
	using namespace std::chrono_literals;

  if (packet.size() < 1)
    throw error_invalid_parameter(format_error, "packet size");
  
  if (packet.size() > 0x10000)
  {
    for (auto&& packet : packet.split(0x10000))    
      send(packet);
    return;
  } 

	static thread_local cobs_encoder encoder;

	encoder.init();
	encoder.write<std::uint16_t>(PACKET_SIGNATURE);
	encoder.write<std::uint16_t>(packet.size());
	encoder.write<std::uint32_t>(packet.crc32());
	encoder.write(packet.data(), packet.size());
	encoder.done();
	
	auto t0 = clock::now();
	auto t1 = t0;

	std::uint32_t sent_bytes;
	do
	{
		if (to != 0u) 
		{
			t1 = clock::now();
			if (t1 - t0 > to * 1ms)
				throw error_send_timeout(format_error);		
		}
		sent_bytes = send_bytes(encoder.data(), encoder.size(), to);	
	}
	while(sent_bytes < encoder.size());
}

auto packet_io_win32::recv(std::uint32_t to) -> packet_buffer<byte>
{
	using clock = std::chrono::high_resolution_clock;
	using namespace std::chrono_literals;

	static thread_local std::vector<uint8_t> temp;
	static thread_local cobs_decoder decoder;
	
	std::uint32_t received_bytes;
	std::uint8_t value;
	
	temp.clear();
	auto t0 = clock::now();
	auto t1 = t0;
	do
	{
	L_retry:
		if (to != 0)
		{
			t1 = clock::now();
			if ((t1 - t0) > (to * 1ms))
				throw error_recv_timeout(format_error);		
		}
		received_bytes = recv_bytes(&value, sizeof(value), to);
		if (received_bytes != sizeof(value))
			goto L_retry;		
		temp.push_back(value);
	}
	while(value != 0);

	decoder.init();
	decoder.write(std::span{ temp });
	decoder.done();
	
	if (decoder.packets().size() != 1)
		throw error_decode_failed(format_error);

	std::span packet_bits{ decoder.packets()[0] };
	
	auto signature = take_one<std::uint16_t>(packet_bits);
	auto packetlen = take_one<std::uint16_t>(packet_bits);
	auto packetcrc = take_one<std::uint32_t>(packet_bits);
	
	if (signature != PACKET_SIGNATURE)
		throw error_bad_packet_magic(format_error);
	
	if (packetlen != packet_bits.size())
		throw error_bad_packet_size(format_error);

	if (packetcrc != crc32(packet_bits))
		throw error_bad_packet_crc(format_error);
	
	return packet_buffer<byte>(
		packet_buffer<>::copy_from_flag, 
		packet_bits 
	);	
}

auto packet_io_win32::send_bytes(const byte* bytes, std::size_t size, std::uint32_t to) -> std::size_t
{
  unsigned long nWritten{ 0 };
  if constexpr (sizeof(size) > sizeof(DWORD)) {
    if (size > std::numeric_limits<DWORD>::max())
      throw error_buffer_too_big(format_error);
  }

	COMMTIMEOUTS ctos;
	RtlSecureZeroMemory(&ctos, sizeof(ctos));
	if (!GetCommTimeouts(m_handle, &ctos))
		throw error_cant_get_timeout(format_error);
	ctos.WriteTotalTimeoutConstant = to;
	ctos.WriteTotalTimeoutMultiplier = 0;
	if (!SetCommTimeouts(m_handle, &ctos))
		throw error_cant_set_timeout(format_error);
	
  if (!WriteFile(m_handle, bytes, (unsigned long)size, &nWritten, nullptr))
    throw error_cant_write_pipe(format_error, GetLastError());
  return nWritten;  
}

auto packet_io_win32::recv_bytes(byte* byte, std::size_t size, std::uint32_t to) -> std::size_t
{
  unsigned long nRead{ 0 };
  if constexpr (sizeof(size) > sizeof(DWORD)) {
    if (size > std::numeric_limits<DWORD>::max())
      size = std::numeric_limits<DWORD>::max();
  }
	
	COMMTIMEOUTS ctos;
	RtlSecureZeroMemory(&ctos, sizeof(ctos));
	if (!GetCommTimeouts(m_handle, &ctos))
		throw error_cant_get_timeout(format_error);
	ctos.ReadIntervalTimeout = 0;
	ctos.ReadTotalTimeoutConstant = to;
	ctos.ReadTotalTimeoutMultiplier = 0;
	if (!SetCommTimeouts(m_handle, &ctos))
		throw error_cant_set_timeout(format_error);

  if (!ReadFile(m_handle, byte, (unsigned long)size, &nRead, nullptr)) 
    throw error_cant_read_pipe(format_error, GetLastError());
  return nRead;
}
