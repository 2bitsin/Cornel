#pragma once

#include <tuple>
#include <concepts>
#include <string>
#include <iterator>

#include <meta/type_list.hpp>
#include <meta/string.hpp>

#include <textio/format/node_static.hpp>
#include <textio/format/node_insert.hpp>
#include <textio/format/optimize.hpp>
#include <textio/format/parse.hpp>
#include <textio/format/variable.hpp>

namespace textio::fmt::detail
{
  template <meta::string String>
  inline constexpr auto format_encode()
  {
    return format_optimize_empty(format_optimize_merge(format_static<meta::string_truncate_v<String>, 0u>()));
  }
  
	
  template <typename Char_type, typename ArgsTuple, typename... NodeN>
  inline auto format_to_impl(vconvert_base<Char_type>& vconv_r, meta::type_list<NodeN...>, ArgsTuple const& args) -> convert_error
  {	
		convert_error error_v { convert_error::none };
		auto const result_v = (((error_v = NodeN::apply(vconv_r, args)) != convert_error::none) && ...);    
    return error_v;
  }
}

namespace textio::fmt
{

	template <meta::string Format_string = meta::string{"{}"}, typename... ArgN>
	auto format_to(FILE* ofile_v, ArgN&&... args_v) -> FILE*
	{
		using namespace detail;
		using char_type = typename decltype(Format_string)::char_type;
		vconvert_cstdio<char_type> vconv_r{ ofile_v };
		auto const error_v = format_to_impl(vconv_r, format_encode<Format_string>(), std::forward_as_tuple(std::forward<ArgN>(args_v)...));
		return ofile_v;
	}
	
	template <meta::string Format_string = meta::string{"{}"}, std::output_iterator<typename decltype(Format_string)::char_type> Output_type, typename... ArgN>
	auto format_to(Output_type output_v, ArgN&&... args_v) -> Output_type
	{
		using namespace detail;
		using char_type = typename decltype(Format_string)::char_type;
		vconvert_iterator<Output_type, char_type> vconv_r{ output_v };
		auto const error_v = format_to_impl(vconv_r, format_encode<Format_string>(), std::forward_as_tuple(std::forward<ArgN>(args_v)...));		
		return output_v;
	}

	template <meta::string Format_string = meta::string{"{}"}, typename... ArgN>
	auto format_to(detail::vconvert_base<typename decltype(Format_string)::char_type>& vconv_r, ArgN&&... args_v) 
		-> detail::vconvert_base<typename decltype(Format_string)::char_type>&
	{
		using namespace detail;
		auto const error_v = format_to_impl(vconv_r, format_encode<Format_string>(), std::forward_as_tuple(std::forward<ArgN>(args_v)...));
		return vconv_r;		
	}
	
/*
  template <meta::string Format_string = meta::string{"{}"}, typename Output_type, typename... ArgN>
  auto format_to(Output_type&& output_v, ArgN&&... args)
  {  
		using output_type = std::remove_cvref_t<Output_type>;
		using char_type = typename decltype(Format_string)::char_type;
		
		using namespace detail;
		if constexpr (std::output_iterator<output_type, char>)
		{
			vconvert_iterator<output_type, char_type> vconv_r { std::forward<Output_type>(output_v) };
			return format_to_impl(vconv_r, format_encode<Format_string>(), std::forward_as_tuple(std::forward<ArgN>(args)...));	
		}
		else if constexpr (std::is_same_v<output_type, FILE*>) 
		{
			vconvert_cstdio<char_type> vconv_r{ std::forward<Output_type>(output_v) };
			return format_to_impl(vconv_r, format_encode<Format_string>(), std::forward_as_tuple(std::forward<ArgN>(args)...));	
		}
		else if constexpr (back_insertable<output_type, char_type>)
		{			
			vconvert_back_insert<char_type> vconv_r{ std::forward<Output_type>(output_v) };
			return format_to_impl(vconv_r, format_encode<Format_string>(), std::forward_as_tuple(std::forward<ArgN>(args)...));
		}
		else
		{
			static_assert(sizeof(Output_type*) == 0, "Output_type is not a valid output type");
		}		
  }*/	
	
	
  template <meta::string Format_string = meta::string{"{}"}, typename AsWhat = std::string, typename... ArgN>
  auto format_as(ArgN&&... args) -> AsWhat
  {
		using namespace detail;
    AsWhat collect;
		vconvert_back_insert<typename decltype(Format_string)::char_type, AsWhat> vconv_r { collect };
    auto const error_v = format_to_impl(vconv_r, format_encode<Format_string>(), std::forward_as_tuple(std::forward<ArgN>(args)...));
		if (error_v != convert_error::none)
			return collect;
		//TODO: What to do in case of error ?
		return AsWhat{};
  }
  
  template <meta::string Format_string = meta::string{"{}"}, typename... ArgN>
  auto format_to(std::FILE* file, ArgN&&... args) noexcept -> detail::convert_error
  {
		using namespace detail;
		vconvert_cstdio<typename decltype(Format_string)::char_type> vconv_r{ file };
    return format_to_impl(vconv_r, format_encode<Format_string>(), std::forward_as_tuple(std::forward<ArgN>(args)...));    
  }
	
  template <meta::string Format_string = meta::string{"{}"}, typename... ArgN>
  auto format_to(std::basic_string<typename decltype(Format_string)::char_type>& ostring_v, ArgN&&... args) noexcept -> detail::convert_error
  {
		using namespace detail;
		using char_type = typename decltype(Format_string)::char_type;
		vconvert_back_insert<char_type, std::basic_string<char_type>> vconv_r{ ostring_v };
    return format_to_impl(vconv_r, format_encode<Format_string>(), std::forward_as_tuple(std::forward<ArgN>(args)...));    
  }
	
}

namespace textio::fmt
{

	
  template <meta::string Format_string>
  struct format_statement
  {
    constexpr format_statement() = default;
    using char_type = typename decltype(Format_string)::char_type;

    static inline constexpr auto format_string_encoded = detail::format_encode<Format_string>();

    template <std::output_iterator<char> OIterator, typename... ArgN>
    static inline auto to(OIterator o_iterator, ArgN&& ... args) -> std::tuple<detail::convert_error, OIterator>
    {
			using namespace detail;
			vconvert_iterator<OIterator, char_type> vconv_r{ o_iterator };
      auto const error_v = format_to_impl(vconv_r, format_string_encoded, std::forward_as_tuple(std::forward<ArgN>(args)...));
			return { error_v, o_iterator };
    }   

    template <typename... ArgN>
    static inline auto to(std::basic_string<char_type>& o_string, ArgN&& ... args) -> std::tuple<detail::convert_error, std::basic_string<char_type>>
    {      
			using namespace detail;
			vconvert_back_insert<char_type, std::basic_string<char_type>> vconv_r{ o_string };
      auto const error_v = format_to_impl(vconv_r, format_string_encoded, std::forward_as_tuple(std::forward<ArgN>(args)...));
			return { error_v, o_string };
    }   

    template <typename... ArgN>
    static inline auto to(FILE* o_file, ArgN&& ... args) -> detail::convert_error
    {
			using namespace detail;			
			vconvert_cstdio<char_type> vconv_r{ o_file };
      return format_to_impl(vconv_r, format_string_encoded, std::forward_as_tuple(std::forward<ArgN>(args)...));
    }   

    template <typename As_type, typename... ArgN>
    static inline auto as(ArgN&& ... args)
    {
      As_type collect;      
      auto const [error_v, value_v] = to(collect, std::forward<ArgN>(args)...);
			return value_v;
    }   

    template <std::output_iterator<char> OIterator, typename... ArgN>
		inline auto operator () (std::output_iterator<char> auto o_iterator, ArgN&& ... args) const -> std::tuple<detail::convert_error, OIterator>
		{
			return to(o_iterator, std::forward<ArgN>(args)...);
		}

    template <typename... ArgN>
    inline auto operator () (std::basic_string<char_type>& o_string, ArgN&& ... args) const -> std::tuple<detail::convert_error, std::basic_string<char_type>>
    {     
      return to(o_string, std::forward<ArgN>(args)...);
    }
    
    template <typename... ArgN>
    inline auto operator () (std::FILE* o_file, ArgN&& ... args) const -> detail::convert_error
    {     
      return to(o_file, std::forward<ArgN>(args)...);
    }
  };

  namespace literals
  {
    template<meta::string Format_string>
    constexpr auto operator "" _fmt()
    {
      return format_statement<Format_string>();
    }
  }
}