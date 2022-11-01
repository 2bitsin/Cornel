#pragma once

#include <cstdint>
#include <cstddef>

#include <string_view>

#include <meta/string.hpp>
#include <textio/general/cstdio_iterator.hpp>
#include <textio/format.hpp>

namespace textio::detail
{
  enum class level_type: int
  {
    none      = -1,
    fatal     = 0,
    error     = 1,
    warning   = 2,
    info      = 3,
    debug     = 4,
    trace     = 5,
    all       = 6
  };

  template <level_type Value>
  struct level_as_string_impl;

  template <> struct level_as_string_impl<level_type::none>    { static constexpr auto value = meta::string_truncate_v< "NONE"    >; };
  template <> struct level_as_string_impl<level_type::fatal>   { static constexpr auto value = meta::string_truncate_v< "FATAL"   >; };
  template <> struct level_as_string_impl<level_type::error>   { static constexpr auto value = meta::string_truncate_v< "ERROR"   >; };
  template <> struct level_as_string_impl<level_type::warning> { static constexpr auto value = meta::string_truncate_v< "WARNING" >; };
  template <> struct level_as_string_impl<level_type::info>    { static constexpr auto value = meta::string_truncate_v< "INFO"    >; };
  template <> struct level_as_string_impl<level_type::debug>   { static constexpr auto value = meta::string_truncate_v< "DEBUG"   >; };
  template <> struct level_as_string_impl<level_type::trace>   { static constexpr auto value = meta::string_truncate_v< "TRACE"   >; };
  template <> struct level_as_string_impl<level_type::all>     { static constexpr auto value = meta::string_truncate_v< "ALL"     >; };

  template <level_type Value>
  static inline constexpr auto level_as_string_v = level_as_string_impl<Value>::value;
}

namespace textio
{
  struct logger_base
  {
    using level_type = textio::detail::level_type;

    template <level_type Level>
    static constexpr auto level_as_string()
    {
      return detail::level_as_string_v<Level>;
    }

    static constexpr auto level_as_string(level_type value) 
      -> std::string_view
    {
      switch (value)
      {
      case level_type::none:  
        return detail::level_as_string_v<level_type::none>;
      case level_type::fatal:   
        return detail::level_as_string_v<level_type::fatal>;
      case level_type::error:   
        return detail::level_as_string_v<level_type::error>;
      case level_type::warning: 
        return detail::level_as_string_v<level_type::warning>;
      case level_type::info:    
        return detail::level_as_string_v<level_type::info>;
      case level_type::debug:   
        return detail::level_as_string_v<level_type::debug>;
      case level_type::trace:   
        return detail::level_as_string_v<level_type::trace>;
      default: return {};     
      }
    }   

    static auto level(level_type value) -> level_type
    {
      return std::exchange (G_current_level, value);
    }

    static auto level() -> level_type
    {
      return G_current_level;
    }

    static auto level_check(level_type value) -> bool
    {
      using ul_type = std::underlying_type_t<level_type>;
      return (ul_type)G_current_level >= (ul_type)value;
    }   
    
  private:
    static inline level_type G_current_level = level_type::info;
  };

  template <meta::string Module = "">
  struct logger_module_base: logger_base
  {   
    
    static auto level(level_type value) -> level_type
    {
      return std::exchange (G_current_level, value);
    }

    static auto level() -> level_type
    {
      return G_current_level;
    }

    static auto level_check(level_type value) -> bool
    {
      if (!logger_base::level_check(value))
        return false;
      using ul_type = std::underlying_type_t<level_type>;
      return (ul_type)G_current_level >= (ul_type)value;
    }   
    
  private:
    static inline level_type G_current_level = level_type::all;
  };

  template <meta::string Module = meta::empty_string_v<char>, typename Output_type = detail::cstdio_iterator>
  struct logger_module: logger_module_base<Module>
  {
    using output_type = Output_type;

    logger_module(Output_type output_handle = Output_type{ }) 
    : m_output { output_handle }
    { }
    
    template<logger_base::level_type Level, meta::string Format_string = "{}", bool Append_new_line = true, typename... Args>    
    auto log(Args&& ... args)  -> logger_module&
    {
      using textio::fmt::format_to;
      
      if constexpr (Level != logger_base::level_type::none)
      {
        if (!logger_module_base<Module>::level_check(Level)) 
        {
          return *this;
        }
      }

      if constexpr (!Module.empty())
      {
        constexpr meta::string format_s { meta::string_truncate_v<"(">, 
                                          meta::string_truncate_v<Module>, 
                                          meta::string_truncate_v<") "> };
        m_output = format_to<format_s>(m_output);
      }

      if constexpr (logger_base::level_type::none != Level
                 && logger_base::level_type::info != Level)
      {
        
        constexpr meta::string format_s { detail::level_as_string_v<Level>, 
                                          meta::string_truncate_v<": "> };

        m_output = format_to<format_s>(m_output);
      }
      
      m_output = format_to<Format_string>(m_output, std::forward<Args>(args)...);

      if constexpr (Append_new_line)
      {
        m_output = format_to<meta::string_truncate_v<"\n">>(m_output);
      }
 
      return *this;
    }
    // TODO: Instead of using boot Append_new_line, add an selectable line ending, e.g. "\n", "\r\n", "\r", etc. that defaults to "\n"
    template<meta::string Format_string = "{}", bool Append_new_line = true, typename... Args>  auto fatal (Args&& ... args)  -> logger_module& { return log<logger_base::level_type::fatal,   Format_string, Append_new_line>(std::forward<Args>(args)...); }
    template<meta::string Format_string = "{}", bool Append_new_line = true, typename... Args>  auto error (Args&& ... args)  -> logger_module& { return log<logger_base::level_type::error,   Format_string, Append_new_line>(std::forward<Args>(args)...); }
    template<meta::string Format_string = "{}", bool Append_new_line = true, typename... Args>  auto warn  (Args&& ... args)  -> logger_module& { return log<logger_base::level_type::warning, Format_string, Append_new_line>(std::forward<Args>(args)...); }
    template<meta::string Format_string = "{}", bool Append_new_line = true, typename... Args>  auto info  (Args&& ... args)  -> logger_module& { return log<logger_base::level_type::info,    Format_string, Append_new_line>(std::forward<Args>(args)...); }
    template<meta::string Format_string = "{}", bool Append_new_line = true, typename... Args>  auto debug (Args&& ... args)  -> logger_module& { return log<logger_base::level_type::debug,   Format_string, Append_new_line>(std::forward<Args>(args)...); }
    template<meta::string Format_string = "{}", bool Append_new_line = true, typename... Args>  auto trace (Args&& ... args)  -> logger_module& { return log<logger_base::level_type::trace,   Format_string, Append_new_line>(std::forward<Args>(args)...); }
    template<meta::string Format_string = "{}", bool Append_new_line = true, typename... Args>  auto write (Args&& ... args)  -> logger_module& { return log<logger_base::level_type::none,    Format_string, Append_new_line>(std::forward<Args>(args)...); }
            
  private:
    output_type m_output;
  };  

  template <meta::string Module, typename Output_type>
  logger_module(Output_type output_i)->logger_module<Module, Output_type>;
}

#define declare_module(Module) static  auto Gmod = textio::logger_module<#Module, textio::detail::cstdio_iterator>{stdout}

extern textio::logger_module<meta::empty_string_v<char>, textio::detail::cstdio_iterator> Glog;
