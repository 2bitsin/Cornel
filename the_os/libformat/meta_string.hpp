#pragma once

#include <cstddef>
#include <cstdint>

#include <stdexcept>
#include <type_traits>

namespace meta
{

	template <size_t Count, typename CharT = char>
	struct string
	{
		using char_type = CharT;
		static inline constexpr auto m_size = Count;
		char_type m_data [m_size];		
		
		constexpr string(char_type const (&data)[m_size])
		{
			for (size_t index = 0u; index < m_size; ++index)			
				m_data [index] = data [index];			
		}		

		constexpr string(auto... chars)
		: m_data{ chars... }
		{}
						
		constexpr auto operator [] (size_t index) const -> char_type
		{
			return index < m_size ? m_data [index] : throw std::out_of_range("Subscript out of range");
		}
				
		static constexpr auto size() -> size_t
		{
			return m_size;
		}		

		static constexpr auto empty() -> bool
		{
			return m_size == 0u;
		}		
		
		template <size_t Offset, size_t... Index>
		constexpr auto substr(std::index_sequence<Index...> what) const -> string<sizeof...(Index), char_type>
		{
			return { (m_data[Offset + Index]) ... };
		}

		template <size_t Offset, size_t NewCount>
		constexpr auto substr() const
		{
			return substr<Offset>(std::make_index_sequence<NewCount>{});
		}

		template <char_type Value>
		constexpr auto find() const -> size_t
		{
			for (size_t index = 0u; index < m_size; ++index) 
			{
				if (m_data[index] == Value)					
				{ return index; }
			}
			return npos;
		}

		constexpr auto find(char_type Value, size_t Offset = 0u) const -> size_t
		{
			for (size_t index = Offset; index < m_size; ++index)
			{
				if (m_data[index] == Value)
				{ return index; }
			}
			return npos;
		}

		static constexpr auto empty_string() -> string<0u, char_type>
		{
			return {};		
		}

		constexpr auto as_string_view() const -> std::basic_string_view<char_type>
		{
			return { m_data, m_size };
		}
				
		static inline constexpr auto npos = size_t(-1);
	};


	template <typename CharT>
	struct string<0u, CharT> 
	{				
		using char_type = CharT;		
		static inline constexpr auto m_size = 0u;

		static constexpr auto size() -> size_t
		{
			return m_size;
		}		

		static constexpr auto empty() -> bool
		{
			return m_size == 0u;
		}		
		
		constexpr auto as_string_view() const -> std::basic_string_view<char_type>
		{
			return { "" };
		}		
	};	
	
	
	template <typename CharT, size_t Count> 
	constexpr auto string_length(CharT const (&data)[Count]) -> size_t
	{
		size_t index { 0u };
		while(data[index])
			++index;
		return index;
	}

	template <typename CharT, size_t Count> string(CharT const (&data)[Count]) -> string<Count, CharT>;
	
	template <typename...CharTs> string(CharTs...chars) -> string<sizeof...(CharTs), std::common_type_t<CharTs...>>;


}