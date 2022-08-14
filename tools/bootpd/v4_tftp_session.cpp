#include "v4_tftp_session.hpp"
#include "v4_tftp_server.hpp"

#include <fstream>
#include <chrono>


bool v4_tftp_session::is_done() const 
{ return m_done; }

void v4_tftp_session::io_thread(v4_tftp_server& parent, v4_address source, v4_tftp_packet::packet_rrq_type request, std::stop_token st)
{
	using namespace std::chrono_literals;
	using namespace std::string_literals;
	using namespace std::string_view_literals;
	
	using std::filesystem::path;
	using std::filesystem::exists;
	using std::filesystem::is_regular_file;

	auto socket_v { m_address.make_udp() };
	auto file_path_v { m_base_dir / request.filename };
	auto block_id_v	{ 0ull };	
	v4_tftp_packet data_v;

	if (!exists(file_path_v))
	{
		data_v.set_error(data_v.file_not_found);
		socket_v.send(data_v, source, 0);
		throw std::runtime_error("File not found: "s + file_path_v.string());
	}

	if (!is_regular_file(file_path_v))
	{
		data_v.set_error(data_v.access_violation);
		socket_v.send(data_v, source, 0);
		throw std::runtime_error("Not a file: "s + file_path_v.string());
	}

	if (request.xfermode != "octet"s && request.xfermode != "netascii"s)
	
	std::ifstream file_v { file_path_v, std::ios::binary };
	
	while (!st.stop_requested())
	{		
		//data_v.set_data(block_id_v & 0xffffu, {});
	}
}

void v4_tftp_session::io_thread(v4_tftp_server& parent, v4_address source, v4_tftp_packet::packet_wrq_type request, std::stop_token st)
{
	using namespace std::chrono_literals;
	auto socket_v = m_address.make_udp();
	while (!st.stop_requested())
	{
		std::this_thread::sleep_for(1s);
	}
}

