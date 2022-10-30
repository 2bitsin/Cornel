#pragma once

#include <cstdint>
#include <cstddef>

#include <string_view>

#include <meta/string.hpp>
#include <textio/general/cstdio_iterator.hpp>
#include <textio/format.hpp>

namespace textio
{
  struct logger_base
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
    static constexpr const auto& level_as_string()
    {
      if constexpr (Value == level_type::none) 
        return meta::string_truncate_v<"NONE">;     
      else if constexpr (Value == level_type::fatal)
        return meta::string_truncate_v<"FATAL">;      
      else if constexpr (Value == level_type::error)
        return meta::string_truncate_v<"ERROR">;              
      else if constexpr (Value == level_type::warning)
        return meta::string_truncate_v<"WARNING">;            
      else if constexpr (Value == level_type::info)
        return meta::string_truncate_v<"INFO">;           
      else if constexpr (Value == level_type::debug)
        return meta::string_truncate_v<"DEBUG">;      
      else if constexpr (Value == level_type::trace)
        return meta::string_truncate_v<"TRACE">;      
      else
        return meta::empty_string_v<char>;
    }
    
    static constexpr auto level_as_string(level_type value) noexcept
      -> std::string_view
    {

      switch (value)
      {
      case level_type::none:    
        return level_as_string<level_type::none>();
      case level_type::fatal:   
        return level_as_string<level_type::fatal>();
      case level_type::error:   
        return level_as_string<level_type::error>();
      case level_type::warning: 
        return level_as_string<level_type::warning>();
      case level_type::info:    
        return level_as_string<level_type::info>();
      case level_type::debug:   
        return level_as_string<level_type::debug>();
      case level_type::trace:   
        return level_as_string<level_type::trace>();
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

    logger_module(Output_type output_handle = Output_type{ }) noexcept
    : m_output { output_handle }
    { }
    
    template<logger_base::level_type Level, meta::string Format_string = "{}", bool Append_new_line = true, typename... Args>    
    auto log(Args&& ... args) noexcept -> logger_module&
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
        static constexpr std::string_view module_s { meta::string_truncate_v<Module> };
        m_output = format_to<"({}) ">(m_output, module_s);
      }

      if constexpr (logger_base::level_type::none != Level
                 && logger_base::level_type::info != Level)
      {
        static constexpr std::string_view level_s { logger_base::level_as_string(Level) };
        m_output = format_to<"{:s}: ">(m_output, level_s);
      }
      
      if constexpr (Append_new_line)
      {
        static constexpr meta::string format_s { meta::string_truncate_v<Format_string>, meta::string_truncate_v<"\n"> };     
        m_output = format_to<format_s>(m_output, std::forward<Args>(args)...);
      }
      else
      {
        m_output = format_to<Format_string>(m_output, std::forward<Args>(args)...);
      }
 
      return *this;
    }

    template<meta::string Format_string = "{}", bool Append_new_line = true, typename... Args>  auto fatal (Args&& ... args) noexcept -> logger_module& { return log<logger_base::level_type::fatal,   Format_string, Append_new_line>(std::forward<Args>(args)...); }
    template<meta::string Format_string = "{}", bool Append_new_line = true, typename... Args>  auto error (Args&& ... args) noexcept -> logger_module& { return log<logger_base::level_type::error,   Format_string, Append_new_line>(std::forward<Args>(args)...); }
    template<meta::string Format_string = "{}", bool Append_new_line = true, typename... Args>  auto warn  (Args&& ... args) noexcept -> logger_module& { return log<logger_base::level_type::warning, Format_string, Append_new_line>(std::forward<Args>(args)...); }
    template<meta::string Format_string = "{}", bool Append_new_line = true, typename... Args>  auto info  (Args&& ... args) noexcept -> logger_module& { return log<logger_base::level_type::info,    Format_string, Append_new_line>(std::forward<Args>(args)...); }
    template<meta::string Format_string = "{}", bool Append_new_line = true, typename... Args>  auto debug (Args&& ... args) noexcept -> logger_module& { return log<logger_base::level_type::debug,   Format_string, Append_new_line>(std::forward<Args>(args)...); }
    template<meta::string Format_string = "{}", bool Append_new_line = true, typename... Args>  auto trace (Args&& ... args) noexcept -> logger_module& { return log<logger_base::level_type::trace,   Format_string, Append_new_line>(std::forward<Args>(args)...); }
    template<meta::string Format_string = "{}", bool Append_new_line = true, typename... Args>  auto write (Args&& ... args) noexcept -> logger_module& { return log<logger_base::level_type::none,    Format_string, Append_new_line>(std::forward<Args>(args)...); }
            
  private:
    output_type m_output;
  };  

  template <meta::string Module, typename Output_type>
  logger_module(Output_type output_i)->logger_module<Module, Output_type>;
}

#define declare_module(Module) static  auto Gmod = textio::logger_module<#Module, textio::detail::cstdio_iterator>{stdout}

extern textio::logger_module<meta::empty_string_v<char>, textio::detail::cstdio_iterator> Glog;
