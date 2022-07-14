#include <filesystem>
#include <stop_token>
#include <sstream>
#include <iostream>
#include <vector>

#include "common/trim.hpp"

template <typename I_Stream_type, typename O_Stream_type>
struct bootserver
{
	using o_stream_type = O_Stream_type;
	using i_stream_type = I_Stream_type;

	bootserver(i_stream_type& is, o_stream_type& os, std::filesystem::path base_dir) 
	:	m_istream { is },
		m_ostream { os },
		m_basedir { std::filesystem::absolute(base_dir) },
		m_currdir { std::filesystem::absolute(base_dir) }
	{}

	void wdt_keep_alive()
	{}

	void list_directory()
	{
		using dit = std::filesystem::directory_iterator;		
		dit curr{ m_currdir }, end;
		
		for (; curr != end; ++curr)
		{
			if (curr->is_directory())
				m_ostream << "D";
			else
				m_ostream << "F";
			m_ostream << " " << curr->path().filename() << "\n";
		}
	}

	void change_directory(std::vector<std::string> const& args)
	{
		if (args.size() < 1)
			throw std::runtime_error("ERROR 001 \"CD requires an argument\"");

		if (!std::filesystem::is_directory(args[0]))
			throw std::runtime_error("ERROR 002 \"CD argument is not a directory\"");					

		auto new_directory = std::filesystem::absolute(m_currdir / args[0]);
		
		if (!new_directory.string().starts_with(m_basedir.string()))
			throw std::runtime_error("ERROR 003 \"Naughty naughty!\"");
		
		m_currdir = new_directory;
	}
		
	int main(std::stop_token stop)
	{
		int next_char;
		std::string line, cmd, arg;
		std::vector<std::string> args;

		while (!stop.stop_requested())
		{
			try
			{
			L_read_line:
				next_char = m_istream.get();
				wdt_keep_alive();
				if (next_char != '\n' && next_char != '\r')
				{
					line.push_back(next_char & 0xff);
					goto L_read_line;
				}			
				
			L_interpret:
				trim(line);

				std::istringstream line_ss{ line };	
				line.clear();
				
				if (!(line_ss >> std::quoted(cmd)))
					goto L_read_line;
				
				while (line_ss >> std::quoted(arg))
					args.push_back(arg);
				
				if (cmd == "LIST")
				{
					list_directory();
					goto L_read_line;
				}

				if (cmd == "CD")
				{
					change_directory(args);
					goto L_read_line;
				}
					
				throw std::runtime_error("ERROR 000 \"unknown or unsupported command\"");
				goto L_read_line;			
			}
			catch (const std::exception& ex)
			{
				m_ostream << ex.what() << "\n";
			}
		}
		return 0;
	}	

private:
	i_stream_type& m_istream;	
	o_stream_type& m_ostream;	
	std::filesystem::path m_basedir;
	std::filesystem::path m_currdir;
};


int main(int argc, char** argv)
{
	bootserver<std::istream, std::ostream> _bs{ std::cin, std::cout, "."};
	
	std::stop_token st;
	return _bs.main(st);	
}