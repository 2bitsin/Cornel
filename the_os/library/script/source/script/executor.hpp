#pragma once

#include <string_view>
#include <vector>
#include <variant>
#include <optional>
#include <charconv>
#include <type_traits>

#include <meta/string.hpp>
#include <textio/format.hpp>

namespace script
{ 		

	template <typename T>
	static inline auto from_string(T& value_v, std::string_view string_v) -> bool
	{
		if constexpr (std::is_same_v<T, bool>)
		{
			if (string_v == "true" || string_v == "True" || string_v == "TRUE") {
				value_v = true;
				return true;
			}
			if (string_v == "false" || string_v == "False" || string_v == "FALSE"){
				value_v = false;
				return true;
			}
			return from_string<int>(value_v, string_v);
		}		
		else if constexpr (std::is_integral_v<T>)
		{			
			auto from_chars_v = std::from_chars(string_v.data(), string_v.data() + string_v.size(), value_v);
			if (from_chars_v.ec != std::errc{})
				return false;
			return true;
		}
		else if constexpr (std::is_convertible_v<std::string_view, T>)
		{
			value_v = T{ string_v };
			return true;
		}
		else if constexpr (std::is_convertible_v<std::string, T>)
		{
			value_v = T{ std::string(string_v) };
			return true;
		}
		else
		{
			return false;
		}
	}
	
	template <typename Implementation, meta::string String, typename ... ArgN>
	struct command_base
	{
		using args_tuple = std::tuple<ArgN...>;
		using args_vec = std::vector<std::string_view>;
		static inline constexpr auto string = meta::string_truncate_v<String>;
		static inline constexpr auto last_arg_index = sizeof...(ArgN) - 1u;
		static inline constexpr auto is_variadic = std::is_same_v<args_vec, std::tuple_element_t<last_arg_index, args_tuple>>;
		
		auto set_arg(auto&& exec, std::size_t index, std::string_view value) -> bool
		{
			if (index < std::size(m_args))
			{
				m_args[index] = std::move(value);
				return true;
			}			
			
			if constexpr (is_variadic)
			{
				index -= last_arg_index;
				if (m_args[last_arg_index].size() <= index)
					m_args[last_arg_index].resize(index + 1);
				m_args[last_arg_index][index] = value;
				return true;
			}
			
			return false;
		}

		template <std::size_t...Index>
		auto done(auto&& exec, std::index_sequence<Index...>)
			-> std::optional<int>
		{
			using namespace textio::fmt;
			std::tuple<ArgN...> args;
			std::size_t curr_arg_i = 0u;
			const auto convert_success_v = ((curr_arg_i = Index, from_string(std::get<Index>(args), m_args[Index])) && ...);
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

		std::string_view m_args[sizeof...(ArgN)];
	};

	template <typename Implementation, meta::string String>
	struct command_base<Implementation, String>
	{
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


	template <typename... Command>
	struct executor
	{				
		using errno_type = int;

		using cmd_init_type = void(executor&);
		using command_var = std::variant<std::monostate, Command...>;
		
		auto begin() noexcept
			-> executor&
		{			
			m_line += 1u;
			m_argn = 0u;
			m_cmd_v = {};
			return *this; 
		}

		auto end() noexcept
			-> executor&
		{ 
			std::visit([this]<typename T>(T&& cmd_v) 
				-> std::optional<int> 
			{
				if constexpr (std::is_same_v<std::decay_t<T>, std::monostate>) {
					return std::nullopt; 
				}
				else {
					return cmd_v.done(*this);
				}
			}, m_cmd_v);
			return *this;
		}
			
		auto emit(std::string_view value) -> executor&
		{ 		
			using std::tuple;
			using std::string_view;
			
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
							cmd_init(*this);				
							return true;
						}
					}					
					return false;
				}
				else 
				{					
					return cmd.set_arg(*this, m_argn++, value);
				}
			}, m_cmd_v);

			if (!set_arg_v)
			{
				if (m_cmd_v.index() != 0u)
					return error("Invalid argument.");
				else
					return error("Invalid command.");
			}
						
			return *this; 
		}
		
		auto error(std::string_view what) noexcept 
			-> executor&
		{	
			using namespace textio::fmt;
			format_to<"{} (on line {}), error: {}\n">(stdout, m_cmd_s, m_line, what);
			return *this; 
		}
			
	private:		
	  std::size_t m_line { 0u };
		std::size_t m_argn { 0u };
		command_var m_cmd_v {};
		std::string_view m_cmd_s {};
	};
	
}