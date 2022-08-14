#include "v4_tftp_session.hpp"
#include "v4_tftp_server.hpp"

#include <common/logger.hpp>

#include <fstream>
#include <chrono>
#include <algorithm>

bool v4_tftp_session::is_done() const 
{ return m_done; }

void v4_tftp_session::io_thread(v4_tftp_server& parent, v4_address remote_client, v4_tftp_packet::type_rrq request, std::stop_token st)
{
	using namespace std::chrono_literals;
	using namespace std::string_literals;
	using namespace std::string_view_literals;
	
	using std::filesystem::path;
	using std::filesystem::exists;
	using std::filesystem::is_regular_file;
	using std::filesystem::file_size;
	using std::chrono::system_clock;

	auto socket_v { m_address.make_udp() };
	auto file_path_v { std::filesystem::absolute (m_base_dir / request.filename) };
	auto block_id_v	{ 0ull };	
	auto block_size_v { 512u };
	auto remaining_size_v { 0ull };
	auto timeout_v { 1s };
	auto read_size_v { block_size_v };

	v4_tftp_packet data_v;

	try
	{
		if (request.xfermode != "octet"s && request.xfermode != "netascii"s) {
			socket_v.send(v4_tftp_packet::make_error(data_v.illegal_operation), remote_client, 0);
			throw std::runtime_error("Unsupported transfer mode: "s + request.xfermode);
		}
		
		if (!exists(file_path_v)) {
			socket_v.send(v4_tftp_packet::make_error(data_v.file_not_found), remote_client, 0);
			throw std::runtime_error("File not found: "s + file_path_v.string());
		}

		if (!is_regular_file(file_path_v)) {
			socket_v.send(v4_tftp_packet::make_error(data_v.file_not_found), remote_client, 0);
			throw std::runtime_error("Not a file: "s + file_path_v.string());
		}
		

		std::ifstream file_v { file_path_v, std::ios::binary };
		std::vector<std::byte> data_v;
		
		remaining_size_v = file_size(file_path_v);
		read_size_v = std::min((std::uintmax_t)block_size_v, remaining_size_v);
		data_v = std::vector<std::byte>(read_size_v);
		file_v.read((char*)data_v.data(), read_size_v);

		while (!st.stop_requested())
		{		
			try
			{
				Glog.info("Sending {} bytes of {} ...", data_v.size(), request.filename);
				socket_v.send(v4_tftp_packet::make_data(block_id_v & 0xffffu, data_v), remote_client, 0);
				auto [from_client, packet_bits] = socket_v.recv(0);

				v4_tftp_packet packet_v(packet_bits);
				if (!packet_v.is<v4_tftp_packet::type_ack>()) {
					socket_v.send(v4_tftp_packet::make_error(v4_tftp_packet::illegal_operation), remote_client, 0);
					throw std::runtime_error(std::format(
						"Expected ACK packet, received : {}"sv, 
						packet_v.to_string()
					));
				}
				
				if (packet_v.as<v4_tftp_packet::type_ack>().block_id != (block_id_v & 0xffffu)) {
					socket_v.send(v4_tftp_packet::make_error(v4_tftp_packet::illegal_operation), remote_client, 0);
					throw std::runtime_error(std::format(
						"Expected ACK to block: {}, received : {}"sv, 
						block_id_v & 0xffffu, 
						packet_v.as<v4_tftp_packet::type_ack>().block_id
					));
				}
				
				if (data_v.size() < block_size_v)
					break;
				
				data_v.clear();
				if (remaining_size_v > 0)
				{
					read_size_v = std::min((std::uintmax_t)block_size_v, remaining_size_v);
					data_v.resize(read_size_v);
					file_v.read((char*)data_v.data(), read_size_v);			
					remaining_size_v -= read_size_v;
				}
				++block_id_v;
			}
			catch(socket_error_timedout)
			{ continue; }
		}
	}
	catch (std::exception const& e)
	{
		Glog.error("{}"s, e.what());
	}
	catch (...)
	{
		Glog.error("Unhandled exception"s);
	}
	m_done.store(true);
	parent.session_notify(this);
}

void v4_tftp_session::io_thread(v4_tftp_server& parent, v4_address source, v4_tftp_packet::type_wrq request, std::stop_token st)
{
	using namespace std::chrono_literals;
	auto socket_v = m_address.make_udp();
	while (!st.stop_requested())
	{
		std::this_thread::sleep_for(1s);
	}
}

