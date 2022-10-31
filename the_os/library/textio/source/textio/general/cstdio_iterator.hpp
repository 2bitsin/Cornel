#pragma once

#include <cstdio>
#include <cerrno>

namespace textio::detail
{
	struct cstdio_iterator
  {
    using iterator_category = std::output_iterator_tag;
    using value_type        = void;
    using pointer           = void;
    using reference         = void;
    using difference_type   = ptrdiff_t;

    auto operator = (const char& val) -> cstdio_iterator&
		{ 
			if (0u != m_errno)
				return *this;
			if (EOF == fputc(val, m_filep))
				m_errno = errno;
			return *this; 
		}

    auto operator = (char&& val) -> cstdio_iterator&
		{ 
			if (0u != m_errno)
				return *this;
			if (EOF == fputc(val, m_filep))
				m_errno = errno;
			return *this; 
		}

    auto operator * ()  -> cstdio_iterator&
		{ return *this; }
		
    auto operator ++ ()  -> cstdio_iterator&
		{ return *this; }

    auto operator ++ (int)  -> cstdio_iterator
		{ return *this; }

		cstdio_iterator(std::FILE* file = stdout)
		: m_filep	{ file },
			m_errno { 0		 }
		{}		

		cstdio_iterator(const cstdio_iterator&) = default;
		cstdio_iterator(cstdio_iterator&&) = default;
		cstdio_iterator& operator = (const cstdio_iterator&) = default;
		cstdio_iterator& operator = (cstdio_iterator&&) = default;

		auto status() const  -> int
		{
			return m_errno;
		}
		
	private:
		std::FILE* m_filep { nullptr };
		int m_errno { 0 };
  }; 

}