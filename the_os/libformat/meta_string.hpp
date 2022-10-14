#pragma once

#include <cstddef>
#include <cstdint>

#include <stdexcept>
#include <type_traits>
#include <concepts>

namespace meta
{
  template <typename T, typename V>
  concept is_array_of = requires (T&& t, V v)
  {
    { t[0] } -> std::convertible_to<V>;
    { *std::begin(t) } -> std::convertible_to<V>;
  };

  template <size_t Count, typename CharT = char>
  struct string;
  

  template <meta::string Input>
  struct string_truncate_impl
  {
    static constexpr auto actual_length = ([]() constexpr {
      size_t index{ 0 };
      for(;index < Input.size(); ++index)
        if (Input[index] == '\0')
          return index;
      return index;
    }) ();

    using type = meta::string<actual_length, typename decltype(Input)::char_type>;
    static inline constexpr auto value = Input.template substr<0, actual_length>();
  };  
  
  template <string Input>
  using string_truncate_t = typename string_truncate_impl<Input>::type;
  
  template <string Input>
  static inline constexpr auto string_truncate_v = string_truncate_impl<Input>::value;
  

  template <size_t Count, typename CharT>
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

    template <typename... String>
    requires (is_array_of<String, char_type> && ...)
    constexpr string(String const& ... s)
    {
      size_t index = 0u;
      ((([this, &index](auto&& s) constexpr { 
        for(auto&& ch: s) 
          m_data[index++] = ch;  
      })(s)), ...);
    }
            
    constexpr auto begin() const { return std::begin(m_data); }
    constexpr auto end() const { return std::end(m_data); }

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
  string(CharT const (&data)[Count]) -> string<Count, CharT>;
  
  template <typename...CharTs>
  string(CharTs...chars) -> string<sizeof...(CharTs), std::common_type_t<CharTs...>>;

  template <typename CharT, size_t... SizeN>
  string(string<SizeN, CharT> const& ... parts) -> string<(parts.size() + ...), CharT>;

  
}