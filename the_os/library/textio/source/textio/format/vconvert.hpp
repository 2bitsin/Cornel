#pragma  once

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <span>
#include <string_view>

#include <textio/format/options.hpp>
#include <textio/format/to_chars.hpp>

namespace textio::fmt::detail
{
  enum class convert_error : int
  {
    none,
    too_large,
    out_of_memory,
    invalid_format,
    io_error
  };

  template <typename Char_type>
  struct vconvert_base
  {

    using char_type = Char_type;
    using string_view = std::basic_string_view<char_type>;

    virtual ~vconvert_base () = default;

    virtual auto put(char_type     value_v) noexcept -> convert_error = 0;
    virtual auto put(string_view   value_v) noexcept -> convert_error = 0;
    virtual auto put(char_type     value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(string_view   value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0; 
    virtual auto put(std::uint8_t  value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(std::int8_t   value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(std::uint16_t value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(std::int16_t  value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(std::uint32_t value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(std::int32_t  value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(std::uint64_t value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(std::int64_t  value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(bool          value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(float         value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(double        value_v, format_options<char_type> const& options_v) noexcept -> convert_error = 0;
  };

  template <typename Char_type>
  struct vconvert: public vconvert_base<Char_type>
  {
    using typename vconvert_base<Char_type>::char_type;
    using typename vconvert_base<Char_type>::string_view;
    using vconvert_base<Char_type>::put;

    auto put(string_view   value_v) noexcept -> convert_error override { return put_string(value_v); }
    auto put(char_type     value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_char(value_v, options_v); }
    auto put(string_view   value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_string(value_v, options_v); }     
    auto put(std::uint8_t  value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_integer(value_v, options_v) ; }
    auto put(std::int8_t   value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_integer(value_v, options_v) ; }
    auto put(std::uint16_t value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_integer(value_v, options_v) ; }
    auto put(std::int16_t  value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_integer(value_v, options_v) ; }
    auto put(std::uint32_t value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_integer(value_v, options_v) ; }
    auto put(std::int32_t  value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_integer(value_v, options_v) ; }
    auto put(std::uint64_t value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_integer(value_v, options_v) ; }
    auto put(std::int64_t  value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_integer(value_v, options_v) ; }
    auto put(bool          value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_float(value_v, options_v) ; }
    auto put(float         value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_float(value_v, options_v) ; } 
    auto put(double        value_v, format_options<char_type> const& options_v) noexcept -> convert_error override { return put_boolean(value_v, options_v) ; }

    inline auto put_integer(auto&& value_v, auto const& options_v) noexcept -> convert_error
    {           
      __debugbreak();
      return convert_error::none;       
    }
    
    inline auto put_float(auto&& value_v, auto const& options_v) noexcept -> convert_error
    {
      __debugbreak();
      return convert_error::none;       
    }

    inline auto put_boolean(auto&& value_v, auto const& options_v) noexcept -> convert_error
    {
      __debugbreak();
      return convert_error::none; 
    }
    
    inline auto put_char(auto&& value_v, auto const& options_v) noexcept -> convert_error
    {
      
      ///////////////////////////////
      // Take care of 'c' format type
      ///////////////////////////////
      using value_type = std::remove_cvref_t<decltype(value_v)>;
      if (options_v.format_type == fmt_type::character)
      {       
        char_type buff [1u];
        if constexpr (!std::is_same_v<value_type, char_type>)
          buff[0u] = static_cast<char_type>(value_v);
        else
          buff[0u] = value_v;
        return put(string_view{ buff }, options_v);
      }
      return put_integer(value_v, options_v);
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

    inline auto put_string(auto&& value_v, auto const& options_v) noexcept -> convert_error
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
  };

  ////////////////////////////////////////////////
  //
  //  BACK_INSERT CONVERTER
  //
  ////////////////////////////////////////////////

  template <typename Char_type, typename Container_type = std::basic_string<Char_type>>
  struct vconvert_back_insert: public vconvert<Char_type>
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
  struct vconvert_cstdio: public vconvert<Char_type>
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
  struct vconvert_iterator: public vconvert<Char_type>
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
  struct vconvert_iterator_range: public vconvert<Char_type>
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