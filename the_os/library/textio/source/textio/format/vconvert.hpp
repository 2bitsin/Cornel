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
      constexpr auto min_number_buffer_size = sizeof(value_type) * 16u;
      auto const min_string_buffer_size = std::max<std::size_t>(min_number_buffer_size,
				options_v.pad_zeros * options_v.width + options_v.prefix_base * 2u + 1u);
			
      ///////////////////////////////
      // Take care of 'c' format type
      ///////////////////////////////
      if constexpr (std::is_same_v<value_type, char_type> || options.format_type == fmt_type::character)
      {       
        char_type buff [1u];
        if constexpr (!std::is_same_v<value_type, char_type>)
          buff[0u] = static_cast<char_type>(value);
        else
          buff[0u] = value;       
        return format_sv::apply(o_iterator, typename format_sv::value_type{ buff, std::size(buff) });
      }

			return convert_error::none;
		}
		
		inline auto put_paddig(char_type value_v, std::size_t count_v) noexcept -> convert_error
		{
			convert_error error_v{ convert_error::none };
			for(auto i = 0u; i < total_padding; ++i) {
				error_v = put(options.fill_char);
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
      if (options_v.width < value.size()) {
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

}