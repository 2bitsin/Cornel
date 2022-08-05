#pragma once

#include <algorithm> 
#include <cctype>
#include <string>
#include <string_view>
#include <locale>


static inline void ltrim(std::string_view& sv)
{
	using namespace std::string_view_literals;
	sv.remove_prefix(sv.find_first_not_of(" \t\n\r\f\v"sv));
}

static inline void rtrim(std::string_view& sv)
{
	using namespace std::string_view_literals;
	sv.remove_suffix(sv.find_last_not_of(" \t\n\r\f\v"sv) + 1);
}

static inline void trim(std::string_view& sv)
{
	ltrim(sv);
	rtrim(sv);
}

// trim from start (in place)
static inline void ltrim(std::string& s) {
	s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
		return !std::isspace(ch);
	}));
}

// trim from end (in place)
static inline void rtrim(std::string& s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

// trim from both ends (in place)
static inline void trim(std::string& s) {
	ltrim(s);
	rtrim(s);
}