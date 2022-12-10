#pragma  once

#include <cstddef>
#include <cstdint>
#include <span>
#include <limits>
#include <type_traits>
#include <string_view>
#include <algorithm>
#include <cctype>
#include <cwctype>
#include <charconv>

#include <textio/format/options.hpp>

namespace textio::fmt::detail
{
  enum class convert_error : int
  {
    none,
    too_large,
    out_of_memory,
    invalid_format,
    io_error,
    invalid_argument,
    not_implemented
  };

  template <typename Char_type>
  struct vconvert_base
  {

    using char_type = Char_type;
    using string_view = std::basic_string_view<char_type>;
    using string = std::basic_string<char_type>;

    virtual ~vconvert_base () = default;

    virtual auto put(char_type        value_v) noexcept -> convert_error = 0;
    virtual auto put(string_view      value_v) noexcept -> convert_error = 0;
    virtual auto put(string_view      value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0; 
    virtual auto put(char const*      value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(char8_t const*   value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(char16_t const*  value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(char32_t const*  value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(wchar_t const*   value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(void const*      value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(char             value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(char8_t          value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(char16_t         value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(char32_t         value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(wchar_t          value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(std::uint8_t     value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(std::int8_t      value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(std::uint16_t    value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(std::int16_t     value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(std::uint32_t    value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(std::int32_t     value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(std::uint64_t    value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(std::int64_t     value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(bool             value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(float            value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(double           value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0;
  };

  template <typename Char_type>
  struct vconvert: public vconvert_base<Char_type>
  {
    using typename vconvert_base<Char_type>::char_type;
    using typename vconvert_base<Char_type>::string_view;
    using typename vconvert_base<Char_type>::string;
    using vconvert_base<Char_type>::put;

    auto put(string_view      value_v) noexcept -> convert_error override { return put_string(value_v); }
    auto put(string_view      value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_string(value_v, options_v) ; }     
    auto put(char const*      value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_pointer(value_v, options_v) ; }
    auto put(char8_t const*   value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_pointer(value_v, options_v) ; }
    auto put(char16_t const*  value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_pointer(value_v, options_v) ; }
    auto put(char32_t const*  value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_pointer(value_v, options_v) ; }
    auto put(wchar_t const*   value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_pointer(value_v, options_v) ; }
    auto put(void const*      value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_pointer(value_v, options_v) ; }
    auto put(char             value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_integer(value_v, options_v) ; }
    auto put(char8_t          value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_integer(value_v, options_v) ; }
    auto put(char16_t         value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_integer(value_v, options_v) ; }
    auto put(char32_t         value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_integer(value_v, options_v) ; }
    auto put(wchar_t          value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_integer(value_v, options_v) ; }
    auto put(std::uint8_t     value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_integer(value_v, options_v) ; }
    auto put(std::int8_t      value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_integer(value_v, options_v) ; }
    auto put(std::uint16_t    value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_integer(value_v, options_v) ; }
    auto put(std::int16_t     value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_integer(value_v, options_v) ; }
    auto put(std::uint32_t    value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_integer(value_v, options_v) ; }
    auto put(std::int32_t     value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_integer(value_v, options_v) ; }
    auto put(std::uint64_t    value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_integer(value_v, options_v) ; }
    auto put(std::int64_t     value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_integer(value_v, options_v) ; }
    auto put(bool             value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_boolean(value_v, options_v) ; }
    auto put(float            value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_float(value_v, options_v) ; } 
    auto put(double           value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_float(value_v, options_v) ; }

    inline auto put_numeric(std::uintmax_t value_v, format_options<char_type> const& options_v, bool negative_v = false) noexcept -> convert_error
    {
      using namespace std::string_literals;
      std::string number_v (129u, '\0');
      //////////////////////////////////////
      // Do the numeric conversion
      //////////////////////////////////////
      auto const result_v = std::to_chars(number_v.data(), number_v.data()+number_v.size(), value_v, options_v.base());
      if (result_v.ec != std::errc()) {
        if (result_v.ec == std::errc::value_too_large) {
          return convert_error::too_large;
        }
        return convert_error::invalid_argument;
      }
      //////////////////////////////////////
      // Upper case format
      //////////////////////////////////////
      if (options_v.is_upper ()) 
      {
        if constexpr (std::is_same_v<char_type, wchar_t> || std::is_same_v<char_type, char16_t>) {
          std::transform(number_v.data(), result_v.ptr, number_v.data(), 
            [](char_type c_v) { return std::towupper(c_v); });
        }
        else {
          std::transform(number_v.data(), result_v.ptr, number_v.data(), 
            [](char_type c_v) { return std::toupper(c_v); });
        }
      }   

      //////////////////////////////////////////////////////////
      // Some calculations related to how much padding is needed
      //////////////////////////////////////////////////////////

      auto const result_size_v = result_v.ptr - number_v.data();     
      auto const prefix_string = options_v.prefix_base ? options_v.prefix_string() : ""s;
      auto const include_signs = (negative_v || options_v.plus_sign);
      auto const prefix_size_v = prefix_string.size() + include_signs*1u;
      auto const padding_len_v = options_v.width - (result_size_v + prefix_size_v);

      ////////////////////////////////////////////////////////
      // Sort out 0 padding and signs and put it all together
      ////////////////////////////////////////////////////////

      string buffer_v;
      
      buffer_v.reserve(std::max<std::size_t>(options_v.width, result_size_v + prefix_size_v) + 1u);

      if (negative_v)
        buffer_v.push_back('-');
      
      else if (options_v.plus_sign) 
        buffer_v.push_back('+');        

      if (options_v.prefix_base) 
        buffer_v.append(prefix_string);
      
      if (options_v.pad_zeros) 
        buffer_v.append(padding_len_v, '0');
            
      buffer_v.append(number_v.data(), result_v.ptr);

      return put_string(string_view{ buffer_v }, options_v.as(fmt_type::string));
    } 

    inline auto put_numeric(std::intmax_t value_v, format_options<char_type> const& options_v) noexcept -> convert_error
    {           
      return put_numeric((std::uintmax_t)std::abs(value_v), options_v, value_v < 0u);
    }
    
    template <typename T>
    inline auto put_integer(T const& value_v, format_options<char_type> const& options_v) noexcept -> convert_error
    {
      //////////////////////////////////////
      // Character format type
      //////////////////////////////////////
      if(( (options_v.format_type == fmt_type::character 
        ||  options_v.format_type == fmt_type::string)
        && (std::intmax_t(value_v) >= std::intmax_t(std::numeric_limits<char_type>::min()) 
        &&  std::intmax_t(value_v) <= std::intmax_t(std::numeric_limits<char_type>::max())))
        || (std::is_same_v<char_type, T> && options_v.format_type == fmt_type::none))       
      {       
        char_type buffer_v[1u] = { (char_type)value_v };
        return put_string(string_view{ buffer_v, 1u }, options_v.as(fmt_type::string));
      }       

      //////////////////////////////////////
      // Floating point format type
      //////////////////////////////////////
      if (options_v.format_as_float())
      {
        if constexpr (sizeof(T) <= sizeof(std::uint32_t)) {
          return put_float((float)value_v, options_v);
        }
        else{
          return put_float((double)value_v, options_v);
        }
      }
      
      //////////////////////////////////////
      // Integer format type
      //////////////////////////////////////
      if (options_v.format_as_integer() || options_v.format_type == fmt_type::none) 
      {
        if constexpr (!std::is_signed_v<T>) {
          return put_numeric((std::uint64_t)value_v, options_v);
        }
        else {
          return put_numeric((std::int64_t)value_v, options_v);
        }
      } 

      //////////////////////////////////////
      // Not supported format type 
      //////////////////////////////////////
      return convert_error::invalid_format;
    }

    inline auto put_boolean(auto&& value_v, format_options<char_type> const& options_v) noexcept -> convert_error 
    {
      if (options_v.format_type == fmt_type::string) {
        if (value_v) {
          return put_string(string_view{ "true" }, options_v.as(fmt_type::string));
        }
        else {
          return put_string(string_view{ "false" }, options_v.as(fmt_type::string));
        }
      }
      return put_numeric((std::uint64_t)value_v, options_v.as(fmt_type::lower_decimal));
    }
        
    inline auto put_paddig(char_type value_v, std::size_t count_v) noexcept -> convert_error
    {
      convert_error error_v{ convert_error::none };
      for(auto i = 0u; i < count_v; ++i) {
        error_v = put(value_v);
        if (error_v != convert_error::none){
          return error_v;
        }       
      } 
      return error_v;
    }

    inline auto put_string(auto&& value_v, format_options<char_type> const& options_v) noexcept -> convert_error
    {
      //////////////////////////////////////
      // String larger then aligment width ?
      //////////////////////////////////////
      if (options_v.width < value_v.size()) {
        return put_string(value_v);
      }

      /////////////////////////////////////////
      // Output an aligned string by padding it
      /////////////////////////////////////////
      const auto total_padding = options_v.width - value_v.size();
      convert_error error_v { convert_error::none };
      switch (options_v.direction)
      {


      /////////////////////////////////
      // Left aligned string
      /////////////////////////////////
      case fmt_align::left:       
        error_v = put(value_v);
        if (error_v != convert_error::none){
          return error_v;
        }
        error_v = put_paddig(options_v.fill_char, total_padding);
        if (error_v != convert_error::none){
          return error_v;
        }
        break;        

        
      /////////////////////////////////
      // Right aligned string
      /////////////////////////////////
      case fmt_align::right:
        error_v = put_paddig(options_v.fill_char, total_padding);
        if (error_v != convert_error::none){
          return error_v;
        }
        error_v = put(value_v);
        if (error_v != convert_error::none){
          return error_v;
        }
        break;

        
      /////////////////////////////////
      // Center aligned string
      /////////////////////////////////
      case fmt_align::center:
        {
          const auto left_padding = total_padding / 2;          
          error_v = put_paddig(options_v.fill_char, left_padding);
          if (error_v != convert_error::none) {
            return error_v;
          }
          error_v = put(value_v);
          if (error_v != convert_error::none) {
            return error_v;
          }
          error_v = put_paddig(options_v.fill_char, total_padding - left_padding);
          if (error_v != convert_error::none) {
            return error_v;
          }
          break;
        }
        
      /////////////////////////////////
      // No alignment
      /////////////////////////////////
      case fmt_align::none:
      default:
        error_v = put(value_v);
        if (error_v != convert_error::none) {
          return error_v;
        }
        break;
      }
      return error_v;
    }     

    inline auto put_string(auto&& value_v) noexcept -> convert_error
    {
      for (auto&& char_v : value_v) {
        auto const error_v = put(char_v);
        if (error_v != convert_error::none)
          return error_v;
      }
      return convert_error::none;
    }
    
    template <typename T>
    inline auto put_pointer(T const* value_v, format_options<char_type> const& options_v) noexcept -> convert_error
    {
      static_assert(sizeof(std::uintptr_t) <= sizeof(std::uintmax_t));
      switch (options_v.format_type)
      {
      case fmt_type::lower_binary:
      case fmt_type::upper_binary:
      case fmt_type::lower_octal:
      case fmt_type::upper_octal:
      case fmt_type::lower_decimal:
      case fmt_type::upper_decimal:
      case fmt_type::lower_hexadecimal:
      case fmt_type::upper_hexadecimal:
        return put_numeric((std::uintmax_t)value_v, options_v);
        
      case fmt_type::lower_pointer:
        return put_numeric((std::uintmax_t)value_v, options_v.as(fmt_type::lower_hexadecimal));

      case fmt_type::upper_pointer:     
        return put_numeric((std::uintmax_t)value_v, options_v.as(fmt_type::upper_hexadecimal));

      case fmt_type::string:
        if constexpr (std::is_same_v<char_type, T>) {
          return put_string(string_view{ value_v ? value_v : "@null" }, options_v.as(fmt_type::string));
        }
        else {
          return convert_error::invalid_format;
        }
        break;
      case fmt_type::none:
        if constexpr (std::is_same_v<char_type, T>) {
          return put_string(string_view{ value_v ? value_v : "@null" }, options_v.as(fmt_type::string));
        }
        else {
          return put_numeric((std::uintmax_t)value_v, options_v.as(fmt_type::lower_hexadecimal));
        }
        break;
      default:
        break;
      }         
      return convert_error::invalid_format;
    }

    inline auto put_float(auto&& value_v, format_options<char_type> const& options_v) noexcept -> convert_error
    {      
      return convert_error::not_implemented;
    }
    
  };

  ////////////////////////////////////////////////
  //
  //  BACK_INSERT CONVERTER
  //
  ////////////////////////////////////////////////
  
  template <typename Container_type, typename U>
  concept back_insertable = requires(Container_type& container_v, U&& value_v, std::basic_string_view<std::remove_cvref_t<U>> const& view_v) {
    { container_v.push_back(std::forward<U>(value_v)) } ;
    { container_v.insert(container_v.end(), view_v.begin(), view_v.end()) };
  };

  template <typename Char_type, typename Container_type = std::basic_string<Char_type>>
  struct vconvert_back_insert final: public vconvert<Char_type>
  {
    using container_type = Container_type;
    using typename vconvert_base<Char_type>::char_type;
    using typename vconvert_base<Char_type>::string_view;

    vconvert_back_insert(container_type& sink_v) noexcept: m_sink(sink_v) {}

    auto put(char_type value_v) noexcept -> convert_error override {
      m_sink.push_back(value_v);
      return convert_error::none;
    }
    auto put(string_view value_v) noexcept -> convert_error override {
      m_sink.insert(m_sink.end(), value_v.begin(), value_v.end());
      return convert_error::none;
    } 

  private:
    container_type &m_sink;
  };

  ////////////////////////////////////////////////
  //
  //  CSTDIO CONVERTER
  //
  ////////////////////////////////////////////////

  template <typename Char_type>
  struct vconvert_cstdio final: public vconvert<Char_type>
  {
    using handle_type = FILE*;
    using typename vconvert_base<Char_type>::char_type;
    using typename vconvert_base<Char_type>::string_view;

    vconvert_cstdio(handle_type sink_v) noexcept: m_sink(sink_v) {}

    auto put(char_type value_v) noexcept -> convert_error override {
      if (std::fputc(value_v, m_sink) == EOF)
        return convert_error::io_error;
      return convert_error::none;
    }

    auto put(string_view value_v) noexcept -> convert_error override {
      if (value_v.empty())
        return convert_error::none;
      auto const written_v = std::fwrite(value_v.data(), 1u, value_v.size(), m_sink);
      if (0u == written_v)
        return convert_error::io_error;
      if (written_v < value_v.size())
        return convert_error::too_large;
      return convert_error::none;
    } 

  private:
    handle_type m_sink;
  };

  ////////////////////////////////////////////////
  //
  //  OUTPUT ITERATOR CONVERTER
  //
  ////////////////////////////////////////////////

  template <typename Output_type, typename Char_type>
  struct vconvert_iterator final: public vconvert<Char_type>
  {
    using typename vconvert_base<Char_type>::char_type;
    using typename vconvert_base<Char_type>::string_view;

    template <typename T> 
    vconvert_iterator (T &&iterator_v) noexcept
    : m_oiterator { std::forward<T>(iterator_v) }
    {}

    vconvert_iterator (vconvert_iterator const&) = default;
    vconvert_iterator (vconvert_iterator &&) = default;

    vconvert_iterator& operator = (vconvert_iterator const&) = default;
    vconvert_iterator& operator = (vconvert_iterator &&) = default;

    auto put(char_type value_v) noexcept -> convert_error override {
      *m_oiterator++ = value_v;     
      return convert_error::none;
    }

    auto put(string_view value_v) noexcept -> convert_error override {
      m_oiterator = std::copy(value_v.begin(), value_v.end(), m_oiterator);
      return convert_error::none;
    } 

  private:
    Output_type m_oiterator;
  };

  ////////////////////////////////////////////////
  //
  //  ITERATOR RANGE CONVERTER
  //
  ////////////////////////////////////////////////

  template <typename Output_type, typename Char_type>
  struct vconvert_iterator_range final: public vconvert<Char_type>
  {
    using typename vconvert_base<Char_type>::char_type;
    using typename vconvert_base<Char_type>::string_view;

    template <typename TB, typename TE> 
    vconvert_iterator_range (TB&& beg_v, TE&& end_v) noexcept
    : m_curr { std::forward<TB>(beg_v) }
    , m_end  { std::forward<TE>(end_v) }
    {}

    vconvert_iterator_range (vconvert_iterator_range const&) = default;
    vconvert_iterator_range (vconvert_iterator_range &&) = default;

    vconvert_iterator_range& operator = (vconvert_iterator_range const&) = default;
    vconvert_iterator_range& operator = (vconvert_iterator_range &&) = default;

    auto put(char_type value_v) noexcept -> convert_error override {
      if (m_curr == m_end)
        return convert_error::too_large;
      *m_curr++ = value_v;
      return convert_error::none;
    }

    auto put(string_view value_v) noexcept -> convert_error override {
      for(auto&& char_v : value_v) {
        auto const error_v = put(char_v);
        if (error_v != convert_error::none)
          return error_v;
      }
      return convert_error::none;
    } 

  private:
    Output_type m_curr;
    Output_type m_end;
  };

}
