#pragma once

#include <string_view>
#include <array>
#include <vector>
#include <variant>
#include <optional>
#include <charconv>
#include <type_traits>
#include <algorithm>
#include <iterator>

#include <meta/string.hpp>
#include <textio/format.hpp>

namespace script::detail
{ 		

	static inline auto bounce_string(auto& value_o, auto const& value_i)
	{
		value_o.clear();
		value_o.reserve(value_i.size());
		bool is_escape = false;
		for (auto&& char_v : value_i)
		{				
			if (!is_escape)
			{
				if (char_v != '\\') {
					value_o.push_back	(char_v);
					continue;	
				}
				is_escape = true;
				continue;
			}
			switch (char_v)
			{
			case 'n': value_o.push_back('\n'); break;
			case 'r': value_o.push_back('\r'); break;
			case 't': value_o.push_back('\t'); break;
			case 'v': value_o.push_back('\v'); break;
			case 'b': value_o.push_back('\b'); break;
			case 'f': value_o.push_back('\f'); break;
			case 'a': value_o.push_back('\a'); break;
			case '\\': value_o.push_back('\\'); break;
			case '\'': value_o.push_back('\''); break;
			case '\"': value_o.push_back('\"'); break;
			default: value_o.push_back(char_v); break;
			}
			is_escape = false;
		}
		if (is_escape) {
			value_o.push_back('\\');
		}
		return true;		
	}

	template <std::size_t Index, typename Exec, typename Tuple, typename Array>
	static inline auto convert_arg(Exec&& exec, Tuple& args_o, Array const& args_i) -> bool
	{
		using namespace std::string_view_literals;
		using namespace textio::fmt;

		using T = std::tuple_element_t<Index, Tuple>;		
		
		auto& value_o = std::get<Index>(args_o);
		auto const& value_i = args_i[Index];
		
		if constexpr (std::is_same_v<T, bool>)
		{
			if (value_i == "true"sv || value_i == "True"sv || value_i == "TRUE"sv) {
				value_o = true; 				
				return true;
			}
			else if (value_i == "false"sv || value_i == "False"sv || value_i == "FALSE"sv){
				value_o = false;
				return true;
			}

			int int_v { 0 };
			auto [end_i, errc] = std::from_chars(value_i.data(), value_i.data() + value_i.size(), int_v);
			if (errc != std::errc()) {
				exec.error(format_as<"Invalid argument '{}'", std::string>(value_i));
				return false;
			}
			value_o = !!int_v;
			return true;
		}		
		else if constexpr (std::is_integral_v<T>)
		{
			auto [end_i, errc] = std::from_chars(value_i.data(), value_i.data() + value_i.size(), value_o);
			if (errc != std::errc()) {
				exec.error(format_as<"Invalid argument '{}'", std::string>(value_i));
				return false;
			}			
			return true;
		}
		else if constexpr (std::is_same_v<T, std::vector<std::string_view>>)
		{
			value_o.clear();
			value_o.reserve(args_i.size() - Index);
			value_o.insert(value_o.end(), args_i.begin() + Index, args_i.end());
			return true;
		}
		else if constexpr (std::is_same_v<T, std::vector<std::string>>)
		{
			value_o.clear();
			value_o.reserve(args_i.size() - Index);
			std::transform(args_i.begin() + Index, args_i.end(), std::back_inserter(value_o), [] (auto&& i) 
			{ 
				std::string o;
				if (bounce_string(o, i))
					return o;
				return std::string(i);
			});
			return true;
		}
		else if constexpr (std::is_same_v<T, std::string>)
		{
			return bounce_string(value_o, value_i);
		}
		else if constexpr (std::is_convertible_v<decltype(value_i), T>)
		{
			value_o = value_i;
			return true;
		}
		else
		{
			exec.error(format_as<"Invalid argument '{}'", std::string>(value_i));
			return false;
		}		
	}
}

namespace script
{	
	template <typename Implementation, meta::string String, typename ... ArgN>
	struct command_base
	{
		using args_tuple = std::tuple<ArgN...>;
		using argsv_vec = std::vector<std::string_view>;
		using argss_vec = std::vector<std::string>;
		static inline constexpr auto string = meta::string_truncate_v<String>;
		static inline constexpr auto last_arg_index = sizeof...(ArgN) - 1u;
		static inline constexpr auto is_variadic 
			 =std::is_same_v<argsv_vec, std::tuple_element_t<last_arg_index, args_tuple>> 
			||std::is_same_v<argss_vec, std::tuple_element_t<last_arg_index, args_tuple>> ;
		 
		command_base() noexcept
		{
			if constexpr (is_variadic) {
				m_args.resize(last_arg_index);
			}
		}
		
		auto set_arg(auto&& exec, std::size_t index, std::string_view value) -> bool
		{
			if constexpr (is_variadic)
			{
				if (index >= std::size(m_args))
					m_args.resize(index + 1u);
			}
			
			if (index < std::size(m_args))
			{
				m_args[index] = std::move(value);
				return true;
			}			
			
			exec.error("Too many arguments");			
			
			return false;
		}

		template <std::size_t...Index>
		auto done(auto&& exec, std::index_sequence<Index...>)
			-> std::optional<int>
		{
			using namespace textio::fmt;
			
			std::size_t curr_arg_i = 0u;
			args_tuple args;
			const auto convert_success_v = ((curr_arg_i = Index, detail::convert_arg<Index>(exec, args, m_args)) && ...);
			if (!convert_success_v) {
				exec.error(format_as<"argument {} is invalid", std::string>(curr_arg_i));
				return std::nullopt;
			}
			return std::apply([&exec, this](auto&&... args) {
				return (*(Implementation*)this).execute(exec, std::forward<decltype(args)>(args)...);
			}, args);
		}

		auto done(auto&& exec) 
			-> std::optional<int>
		{
			return done(std::forward<decltype(exec)>(exec), std::make_index_sequence<sizeof...(ArgN)>{});
		}
		
		using argument_array_type = std::conditional_t<!is_variadic, 
			std::array<std::string_view, sizeof...(ArgN)>, 
			std::vector<std::string_view>>;
		
		argument_array_type m_args;		
	};

	template <typename Implementation, meta::string String>
	struct command_base<Implementation, String>
	{
		static inline constexpr auto string = meta::string_truncate_v<String>;

		auto set_arg(auto&& exec, std::size_t index, std::string_view value) -> bool
		{
			exec.error("command does not accept any arguments.");
			return false;
		}

		auto done(auto&& exec)
			-> std::optional<int>
		{
			return (*(Implementation*)this).execute(exec);
		}
	};


	template <typename Implementation, typename... Command>
	struct executor
	{				
		using status_type = int;

		using cmd_init_type = void(executor&);
		using command_var = std::variant<std::monostate, Command...>;
		using exit_var = std::variant<std::monostate, status_type>;

		using base = executor<Implementation, Command...>;

		auto self() noexcept 
			-> Implementation&
		{
			return (Implementation&)*this;
		}
		
		auto begin() noexcept
			-> Implementation&
		{			
			m_line += 1u;
			m_argn = 0u;
			m_cmd_v = {};
			m_cmd_s = {};
			return self();
		}		

		auto end() noexcept
			-> Implementation&
		{ 
			m_exit_v = std::visit([this]<typename T>(T&& cmd_v) 
				-> std::optional<status_type> 
			{
				if constexpr (std::is_same_v<std::decay_t<T>, std::monostate>) {
					return std::nullopt; 
				}
				else {
					return cmd_v.done(self());
				}
			}, m_cmd_v);
			return self();
		}
			
		auto emit(std::string_view value) 
			-> Implementation&
		{ 		
			using std::tuple;
			using std::string_view;
			// TODO : Implement sorted hash list with binary search for O(log n) lookup, possibly a hashmap later
			static constexpr tuple<string_view, cmd_init_type*> init_table [] = 
			{				
				(tuple{ 
					string_view{ Command::string }, 
					[] (executor& ex) { 
						ex.m_cmd_v = Command{};
						ex.m_cmd_s = Command::string;
					}} 
				)...
			};
								
			const auto set_arg_v = 
				std::visit([this, &value] (auto&& cmd) 
			{ 
				if constexpr (std::is_same_v<std::decay_t<decltype(cmd)>, std::monostate>)
				{
					for (auto&& [cmd_string, cmd_init] : init_table)
					{
						if (value == cmd_string)
						{
							cmd_init(self());				
							return true;
						}
					}					
					return false;
				}
				else 
				{					
					return cmd.set_arg(self(), m_argn++, value);
				}
			}, m_cmd_v);

			if (!set_arg_v)
			{
				if (m_cmd_v.index() != 0u)
					return self().error("Invalid argument.");
				else
					return self().error("Invalid command.");
			}
						
			return self();
		}
				
		auto error(std::string_view what) noexcept 
			-> Implementation&
		{	
			using namespace textio::fmt;
			format_to<"{} (on line {}), error: {}\n">(self().stdout_handle(), m_cmd_s, m_line, what);
			return self();
		}

		auto stdout_handle() const noexcept 
		{
			return stdout;
		}
			
	private:		
	  std::size_t m_line { 0u };
		std::size_t m_argn { 0u };
		command_var m_cmd_v {};
		std::string_view m_cmd_s {};
		std::optional<status_type> m_exit_v {};
	};
	
}