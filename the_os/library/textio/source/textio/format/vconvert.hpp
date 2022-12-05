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
    out_of_memory
  };

  template <typename Char_type>
  struct vconvert_base
  {

    using char_type = Char_type;
    using string_view = std::basic_string_view<char_type>4;

    virtual ~vconvert_base () = default;

		virtual auto put(char_type     value_v) noexcept -> convert_error = 0;
    virtual auto put(string_view   value_v) noexcept -> convert_error = 0;
    virtual auto put(char_type     value_v, format_options<char_type     , char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(string_view   value_v, format_options<string_view   , char_type> const& options_v) noexcept -> convert_error = 0; 
    virtual auto put(std::uint8_t  value_v, format_options<std::uint8_t  , char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(std::int8_t   value_v, format_options<std::int8_t   , char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(std::uint16_t value_v, format_options<std::uint16_t , char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(std::int16_t  value_v, format_options<std::int16_t  , char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(std::uint32_t value_v, format_options<std::uint32_t , char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(std::int32_t  value_v, format_options<std::int32_t  , char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(std::uint64_t value_v, format_options<std::uint64_t , char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(std::int64_t  value_v, format_options<std::int64_t  , char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(bool          value_v, format_options<bool          , char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(float         value_v, format_options<float         , char_type> const& options_v) noexcept -> convert_error = 0;
    virtual auto put(double        value_v, format_options<double        , char_type> const& options_v) noexcept -> convert_error = 0;
  };

	template <typename Char_type>
	struct vconvert: vconvert_base<Char_type>
	{
		using typename vconvert_base<Char_type>::char_type;

    auto put(string_view   value_v) noexcept -> convert_error override { return put_string(value_v); }
    auto put(char_type     value_v, format_options<char_type     , char_type> const& options_v) noexcept -> convert_error override { return put_char(value_v, options_v); }
		auto put(string_view   value_v, format_options<string_view   , char_type> const& options_v) noexcept -> convert_error override { return put_string(value_v, options_v); } 		
    auto put(std::uint8_t  value_v, format_options<std::uint8_t  , char_type> const& options_v) noexcept -> convert_error override { return put_integer(value_v, options_v) ; }
    auto put(std::int8_t   value_v, format_options<std::int8_t   , char_type> const& options_v) noexcept -> convert_error override { return put_integer(value_v, options_v) ; }
    auto put(std::uint16_t value_v, format_options<std::uint16_t , char_type> const& options_v) noexcept -> convert_error override { return put_integer(value_v, options_v) ; }
    auto put(std::int16_t  value_v, format_options<std::int16_t  , char_type> const& options_v) noexcept -> convert_error override { return put_integer(value_v, options_v) ; }
    auto put(std::uint32_t value_v, format_options<std::uint32_t , char_type> const& options_v) noexcept -> convert_error override { return put_integer(value_v, options_v) ; }
    auto put(std::int32_t  value_v, format_options<std::int32_t  , char_type> const& options_v) noexcept -> convert_error override { return put_integer(value_v, options_v) ; }
    auto put(std::uint64_t value_v, format_options<std::uint64_t , char_type> const& options_v) noexcept -> convert_error override { return put_integer(value_v, options_v) ; }
		auto put(std::int64_t  value_v, format_options<std::int64_t  , char_type> const& options_v) noexcept -> convert_error override { return put_integer(value_v, options_v) ; }
	  auto put(bool          value_v, format_options<bool          , char_type> const& options_v) noexcept -> convert_error override { return put_float(value_v, options_v) ; }
		auto put(float         value_v, format_options<float         , char_type> const& options_v) noexcept -> convert_error override { return put_float(value_v, options_v) ; } 
		auto put(double        value_v, format_options<double        , char_type> const& options_v) noexcept -> convert_error override { return put_boolean(value_v, options_v) ; }

		inline auto put_integer(auto&& value_v, auto const& options_v) noexcept -> convert_error
		{
			return convert_error::none;				
		}
		
		inline auto put_float(auto&& value_v, auto const& options_v) noexcept -> convert_error
		{
			return convert_error::none;				
		}

		inline auto put_boolean(auto&& value_v, auto const& options_v) noexcept -> convert_error
		{
			return convert_error::none;	
		}
		
		inline auto put_char(auto&& value_v, auto const& options_v) noexcept -> convert_error
		{
			return convert_error::none;
		}

		inline auto put_string(auto&& value_v, auto const& options_v) noexcept -> convert_error
		{
      //////////////////////////////////////
      // String larger then aligment width ?
      //////////////////////////////////////
      if (options_v.width < value.size()) {
        return std::copy(value.begin(), value.end(), o_iterator);
      }

      /////////////////////////////////////////
      // Output an aligned string by padding it
      /////////////////////////////////////////
      const auto total_padding = options.width - value.size();
      switch (options.direction)
      {
      case fmt_align::left:
        o_iterator = std::copy(value.begin(), value.end(), o_iterator);
        return std::fill_n(o_iterator, total_padding, options.fill_char);
      case fmt_align::right:
        o_iterator = std::fill_n(o_iterator, total_padding, options.fill_char);
        return std::copy(value.begin(), value.end(), o_iterator);
      case fmt_align::center:
        {
          const auto left_padding = total_padding / 2;
          const auto right_padding = total_padding - left_padding;
          o_iterator = std::fill_n(o_iterator, left_padding, options.fill_char);
          o_iterator = std::copy(value.begin(), value.end(), o_iterator);
          return std::fill_n(o_iterator, right_padding, options.fill_char);
        }
      case fmt_align::none:
      default:
        return std::copy(value.begin(), value.end(), o_iterator);
      }
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

}