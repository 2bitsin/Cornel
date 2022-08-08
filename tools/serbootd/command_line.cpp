#include "command_line.hpp"

void command_line::parse(std::string_view line, std::vector<std::string>& parts)
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

	auto next_part = [&] { parts.emplace_back(std::move(current_part)); };
	auto next_state = [&](S_type value) { prev_state = curr_state; curr_state = value; };

	while (!line.empty())
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
			current_part.push_back(ch);
			next_state(S_Normal);
			break;
		}
	}
	if (!current_part.empty())
		next_part();
}
