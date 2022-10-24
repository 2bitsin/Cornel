#pragma once

#include <string_view>
#include <vector>
#include <iostream>

#include <textio/meta/string.hpp>

namespace script
{ 
	
	template <meta::string>
	struct descriptor
	{};

	template <descriptor... D>
	struct executor
	{				
		using errno_type = int;

		inline auto begin() -> executor&
		{
			m_state = 0u;
			m_line += 1u;
			return *this; 
		}

		inline auto end() -> executor&
		{ 			
			return *this; 
		}
		
		inline auto emit(std::string_view str) -> executor&
		{
			
			return *this; 
		}
		
		inline auto error(std::string_view what) noexcept -> executor&
		{			
			return *this;
		}
			
	private:
		std::size_t m_state	{ 0u };
	  std::size_t m_line { 0u };
	};
	
}