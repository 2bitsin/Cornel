#include <filesystem>
#include <iostream>
#include <sstream>
#include <vector>
#include <stop_token>

#include "error.hpp"
#include "common/cxhash.hpp"
#include "common/trim.hpp"
#include "common/case.hpp"



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

	void getline(std::string& line)
	{
		line.clear();
		int next_char;
		while (true)
		{
			next_char = m_istream.get();
			if (next_char < 0)
				throw error_bad_stream(format_error, next_char);
			if (next_char == '\n' || next_char == '\r')
				continue;
			break;
		}
		line.push_back(next_char);
		while (true)
		{
			next_char = m_istream.get();
			if (next_char < 0)
				throw error_bad_stream(format_error, next_char);
			if (next_char == '\n' || next_char == '\r')
				break;			
			line.push_back(next_char);			
		}
	}

	void parse(std::string_view line, std::vector<std::string>& parts)
	{
		std::string current_part;
		parts.clear();
		
		enum S_type
		{
			S_Normal,
			S_Quotes,
			S_Escape,
			S_Whitespace
		};

		S_type curr_state{ S_Normal };
		S_type prev_state{ S_Normal };
		int ch{ 0 };

		auto next_part	= [&] { parts.emplace_back(std::move(current_part)); };
		auto next_state = [&] (S_type value) { prev_state = curr_state; curr_state = value; };

		while (!line.empty ())
		{
			ch = line.front();
			line = line.substr(1);
			switch (curr_state)
			{
			case S_Normal: // Normal

				// Transition to quotes
				if (ch == '\"') {
					if (prev_state != S_Whitespace)
						next_part();
					next_state(S_Quotes);
					break;
				}

				// Transition to whitespace
				if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
					next_part();						
					next_state(S_Whitespace);
					break;
				}

				// Stay in current state	
				current_part.push_back(ch);
				break;

			case S_Quotes: // Inside quotes

				// Transition out of quotes
				if (ch == '"') { 
					next_part();
					next_state(S_Normal);
					break;
				}

				// Transition into escape
				if (ch == '\\') {
					next_state(S_Escape);
					break;
				}

				// Stay in current state
				current_part.push_back(ch);
				break;

 			case S_Escape: // Escaping
				if (ch == 'n') current_part.push_back('\n');
				else if (ch == 'r') current_part.push_back('\r');
				else if (ch == '\\')  current_part.push_back('\\');
				else {
					current_part.push_back('\\');
					current_part.push_back(ch);
				}
				// Transition out of escape back into quotes
				next_state(S_Quotes);
				break;

			case S_Whitespace: // White spaces
				if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r')
					break;
				if (ch == '\"') {
					next_state(S_Quotes);
					break;
				}
				current_part.push_back (ch);
				next_state(S_Normal);
				break;
			}
		}
		if (!current_part.empty())
			next_part();
	}
	
	void CMD_ls(std::vector<std::string> const& args)
	{}	

	void CMD_cwd(std::vector<std::string> const& args)
	{}	

	void CMD_pwd(std::vector<std::string> const& args)
	{}	

	void CMD_get(std::vector<std::string> const& args)
	{}	

	void CMD_put(std::vector<std::string> const& args)
	{}	

	void CMD_bye(std::vector<std::string> const& args)
	{}	

		
	int main(std::stop_token stop)
	{
		while (!stop.stop_requested())
		{
			std::string line;
			std::vector<std::string> args;
			try
			{
				getline (line);
				parse (line, args);

				if (args.empty())
					throw error_bad_format(format_error, line);

				line.clear();
				switch (cxhash(lowercase(args[0])))
				{
				case "ls"_cxhash	: CMD_ls  (args, line); break;
				case "cd"_cxhash	: CMD_cwd (args, line); break;
				case "pwd"_cxhash	: CMD_pwd (args, line); break;
				case "get"_cxhash : CMD_get (args, line); break;
				case "put"_cxhash : CMD_put (args, line); break;
				case "bye"_cxhash : CMD_bye (args, line); break;
				default:
					throw error_bad_command(format_error,line);
				}

				m_ostream << 0 << " SUCCESS"
			}
			catch (const generic_error& ex)
			{
				m_ostream << ex.code() " ERROR " << " \"" << ex.what() << "\"\n";
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
	bootserver _bs{ std::cin, std::cout, "." };
	
	std::stop_token st;

	return _bs.main(st);	
}