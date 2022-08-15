#include "tftp_session_v4.hpp"
#include "tftp_server_v4.hpp"
#include "tftp_reader.hpp"

#include <common/logger.hpp>

#include <fstream>
#include <chrono>
#include <algorithm>

bool tftp_session_v4::is_done() const 
{ return m_done; }

void tftp_session_v4::io_thread(tftp_server_v4& parent, address_v4 remote_client, tftp_packet::type_rrq request, std::stop_token st)
{
	using namespace std::chrono_literals;
	using namespace std::string_literals;
	using namespace std::string_view_literals;
	using namespace std::filesystem;	
	using std::chrono::system_clock;

	auto file_path_v { std::filesystem::absolute (m_base_dir / request.filename) };
	auto socket_v { m_address.make_udp() };

	try
	{
		if (request.xfermode != "octet"s && request.xfermode != "netascii"s) {
			socket_v.send(tftp_packet::make_error(tftp_packet::illegal_operation), remote_client, 0);
			throw std::runtime_error("Unsupported transfer mode: "s + request.xfermode);
		}
		
		if (!exists(file_path_v)) {
			socket_v.send(tftp_packet::make_error(tftp_packet::file_not_found), remote_client, 0);
			throw std::runtime_error("File not found: "s + file_path_v.string());
		}

		if (!is_regular_file(file_path_v)) {
			socket_v.send(tftp_packet::make_error(tftp_packet::file_not_found), remote_client, 0);
			throw std::runtime_error("Not a file: "s + file_path_v.string());
		}
		
		tftp_reader reader_v (file_path_v, 512u);		
		Glog.info("Sending {} ({} bytes) to '{}' ... ",  request.filename, reader_v.total_size(), remote_client.to_string());
		while (!st.stop_requested())
		{		
			try
			{			
				socket_v.send(reader_v.data(), remote_client, 0);
				
				auto [from_client, packet_bits] = socket_v.recv(0);
				
				tftp_packet packet_v(packet_bits);
				
				if (!packet_v.is<tftp_packet::type_ack>()) {
					socket_v.send(tftp_packet::make_error(tftp_packet::illegal_operation), remote_client, 0);
					throw std::runtime_error(std::format("Expected ACK packet, received : {}"sv, packet_v.to_string()));
				}
				
				if (packet_v.as<tftp_packet::type_ack>().block_id != (reader_v.number() & 0xffffu)) {
					socket_v.send(tftp_packet::make_error(tftp_packet::illegal_operation), remote_client, 0);
					throw std::runtime_error(std::format("Expected ACK to block: {}, received : {}"sv, reader_v.number() & 0xffffu, packet_v.as<tftp_packet::type_ack>().block_id));
				}
				
				if (reader_v.last())
					break;				
				reader_v.next();				
			}
			catch(socket_error_timedout)
			{ continue; }
		}
		Glog.info("Finished sending {} to '{}' ... ", request.filename, remote_client.to_string());
	}
	catch (std::exception const& e)
	{ Glog.error("{}"s, e.what()); }
	catch (...)
	{ Glog.error("Unhandled exception"s); }
	m_done.store(true);
	parent.session_notify(this);
}

void tftp_session_v4::io_thread(tftp_server_v4& parent, address_v4 source, tftp_packet::type_wrq request, std::stop_token st)
{
	using namespace std::chrono_literals;
	auto socket_v = m_address.make_udp();
	while (!st.stop_requested())
	{
		std::this_thread::sleep_for(1s);
	}
}

