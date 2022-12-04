#pragma once

#include <type_traits>
#include <memory>
#include <span>
#include <cstring>
#include <string_view>
#include <concepts>

namespace utils
{
  /////////////////////
  // first_true_value
  /////////////////////

  template <typename Arg0, typename... ArgN>  
  static inline constexpr auto first_true_value(Arg0&& arg0, ArgN&&... argn) 
    -> std::common_type_t<Arg0, ArgN...>
  {   
    if (arg0 || !sizeof...(ArgN)) {
      return std::forward<Arg0>(arg0);
    }
    if constexpr (sizeof...(ArgN)) {
      return first_true_value(std::forward<ArgN>(argn)...);
    }   
  }

  template <typename Arg0, typename... ArgN>  
  requires (std::is_same_v<Arg0, ArgN> && ...)
  static inline constexpr auto&& first_true_value(Arg0&& arg0, ArgN&&... argn)    
  {   
    if (arg0 || !sizeof...(ArgN)) {
      return std::forward<Arg0>(arg0);
    }
    if constexpr (sizeof...(ArgN)) {
      return first_true_value(std::forward<ArgN>(argn)...);
    }   
  }

  //////////////
  // is_one_of
  //////////////

  template <auto... ValueN>
  static inline constexpr auto is_one_of(auto&& value) -> bool
  {  
    return ((value == ValueN) || ...);
  }
  
  static inline constexpr auto is_one_of(auto&& value, auto&&... value_n) -> bool
  {  
    return ((value == value_n) || ...);
  }

  //////////////
  // is_none_of
  //////////////

  template <auto... ValueN>
  static inline constexpr auto is_none_of(auto&& value) -> bool
  {  
    return ((value != ValueN) && ...);
  }

  static inline constexpr auto is_none_of(auto&& value, auto&&... value_n) -> bool
  {  
    return ((value != value_n) && ...);
  }

  ////////////////////////////////
  // as_bytes, as_writable_bytes
  ////////////////////////////////

  template <typename T>
  requires (std::is_trivial_v<T>)
  auto as_bytes(T const& what) 
    -> std::span<const std::byte>
  {
    return std::as_bytes(std::span{ &what, 1u });
  }

  template <typename T>
  requires (std::is_trivial_v<T>)
  auto as_writable_bytes(T& what) 
    -> std::span<std::byte>
  {
    return std::as_writable_bytes(std::span{ &what, 1u });
  }

  template <typename T, std::size_t N>
  requires (std::is_trivial_v<T>)
  auto as_bytes(T const (&what) [N]) 
    -> std::span<const std::byte>
  {
    return std::as_bytes(std::span{ what });
  }

  template <typename T, std::size_t N>
  requires (std::is_trivial_v<T>)
  auto as_writable_bytes(T (&what) [N])
    -> std::span<std::byte>
  {
    return std::as_writable_bytes(std::span{ what });
  }

  template <typename T, typename U>
  requires (std::is_trivial_v<T> && std::is_trivial_v<U> && sizeof (T) == sizeof (U))
  constexpr inline auto as_chars(std::span<U const> what_v) noexcept -> std::basic_string_view<T>
  {
    return { (T const *)what_v.data(), what_v.size() };
  }

  template <typename T, typename U>
  requires (std::is_trivial_v<T> && std::is_trivial_v<U> && sizeof (T) == sizeof (U))
  constexpr inline auto as_chars(std::span<U> what_v) noexcept -> std::basic_string_view<T>
  {
    return { (T *)what_v.data(), what_v.size() };
  }

  template <typename Value_type>
  requires (std::is_trivial_v<Value_type>) 
  static inline auto clone_from_bytes(std::span<const std::byte> bytes_v) -> std::unique_ptr<Value_type>
  {
    if (bytes_v.size() < sizeof (Value_type))
      return nullptr;
    auto value_p = std::make_unique<Value_type>();
    std::memcpy(value_p.get(), bytes_v.data(), sizeof (Value_type));
    return std::move (value_p); 
  }

  template <typename Value_type>
  requires (std::is_trivial_v<Value_type>) 
  static inline auto clone_from_bytes(std::span<const std::byte> bytes_v, std::size_t offset_v) -> std::unique_ptr<Value_type>
  {
    if (offset_v >= bytes_v.size() || bytes_v.size() - offset_v < sizeof (Value_type))
      return nullptr;
    return clone_from_bytes<Value_type> (bytes_v.subspan (offset_v));
  }

  template <typename Value_type>
  requires (std::is_trivial_v<Value_type>) 
  static inline auto clone_from_bytes(std::span<const std::byte> bytes_v, std::size_t offset_v, std::size_t count_v) -> std::unique_ptr<Value_type>
  {
    if (offset_v >= bytes_v.size() || bytes_v.size() - offset_v < sizeof (Value_type))
      return nullptr;
    return clone_from_bytes<Value_type> (bytes_v.subspan (offset_v, std::min(bytes_v.size() -  offset_v, count_v)));
  }

  template <typename Value_type>
  requires (std::is_trivial_v<Value_type>)
  static inline auto partial_copy_from_bytes(Value_type& target_v, std::span<const std::byte> bytes_v) -> bool
  {
    if (bytes_v.size() < sizeof (Value_type)) {
      std::memset(&target_v, 0, sizeof (Value_type));   
      std::memcpy(&target_v, bytes_v.data(), bytes_v.size());
      return false;
    }
    std::memcpy(&target_v, bytes_v.data(), sizeof(Value_type));   
    return true;
  }

  template <typename Value_type>
  requires (std::is_trivial_v<Value_type>)
  static inline auto partial_copy_from_bytes(Value_type& target_v, std::span<const std::byte> bytes_v, std::size_t offset_v) -> bool
  {
    if (offset_v >= bytes_v.size())
      return partial_copy_from_bytes(target_v, {});
    return partial_copy_from_bytes(target_v, bytes_v.subspan(offset_v));
  }

  template <typename Value_type>
  requires (std::is_trivial_v<Value_type>)
  static inline auto partial_copy_from_bytes(Value_type& target_v, std::span<const std::byte> bytes_v, std::size_t offset_v, std::size_t size_v) -> bool
  {
    if (offset_v >= bytes_v.size())
      return partial_copy_from_bytes(target_v, {});
    return partial_copy_from_bytes(target_v, bytes_v.subspan(offset_v, std::min(bytes_v.size(), size_v)));
  }

  template <typename Value_type>
  requires (std::is_trivial_v<Value_type>) 
  static inline auto partial_clone_from_bytes(std::span<const std::byte> bytes_v) 
    -> std::unique_ptr<Value_type>
  {
    auto value_p = std::make_unique<Value_type>();
    if (partial_copy_from_bytes(*value_p, bytes_v))
      return value_p;
    return nullptr;
  }

  template <typename Value_type>
  requires (std::is_trivial_v<Value_type>) 
  static inline auto partial_clone_from_bytes(std::span<const std::byte> bytes_v, std::size_t offset_v) 
    -> std::unique_ptr<Value_type>
  {
    auto value_p = std::make_unique<Value_type>();
    if (partial_copy_from_bytes(*value_p, bytes_v, offset_v))     
      return value_p;
    return nullptr;
  }
  
  template <typename Value_type>
  requires (std::is_trivial_v<Value_type>) 
  static inline auto partial_clone_from_bytes(std::span<const std::byte> bytes_v, std::size_t offset_v, std::size_t size_v) 
    -> std::unique_ptr<Value_type>
  {
    auto value_p = std::make_unique<Value_type>();
    if (partial_copy_from_bytes(*value_p, bytes_v, offset_v, size_v))
      return value_p;
    return nullptr;
  }
  
  template <typename Value_type>
  requires (std::is_trivial_v<Value_type>)
  static inline auto from_bytes(std::span<const std::byte> bytes_v) 
    -> std::span<const Value_type> 
  {   
    if (bytes_v.size() < sizeof(Value_type))
      return {};
    return { 
      (Value_type const*)bytes_v.data(), 
      bytes_v.size()/sizeof(Value_type) 
    };
  }

  template <typename Value_type>
  requires (std::is_trivial_v<Value_type>)
  static inline auto from_bytes(std::span<const std::byte> bytes_v, std::size_t offset_v)
    -> std::span<const Value_type>
  {
    if (offset_v >= bytes_v.size() || bytes_v.size() - offset_v < sizeof(Value_type))
      return {};
    return from_bytes<Value_type>(bytes_v.subspan(offset_v));
  }

  template <typename Value_type>
  requires (std::is_trivial_v<Value_type>)
  static inline auto from_bytes(std::span<const std::byte> bytes_v, std::size_t offset_v, std::size_t size_v)
    -> std::span<const Value_type>
  {
    if (offset_v >= bytes_v.size() || bytes_v.size() - offset_v < sizeof(Value_type))
      return {};
    return from_bytes<Value_type>(bytes_v.subspan(offset_v, std::min(bytes_v.size() - offset_v, size_v)));
  }

  template <typename U>
  auto safe_subspan(std::span<const U> bytes_v, std::size_t offset_v, std::size_t size_v) -> std::span<const U>
  {
    if (offset_v >= bytes_v.size())
      return {};
    return bytes_v.subspan(offset_v, std::min(bytes_v.size() - offset_v, size_v));    
  }

  template <typename U>
  auto safe_subspan(std::span<U> bytes_v, std::size_t offset_v, std::size_t size_v) -> std::span<U>
  {
    if (offset_v >= bytes_v.size())
      return {};
    return bytes_v.subspan(offset_v, std::min(bytes_v.size() - offset_v, size_v));     	
	}

	template<typename U, auto N, typename... T>
	requires (sizeof...(T) <= N && ((std::is_convertible_v<U, T>) && ...)) 
	static inline constexpr auto compare_array_to(U const (&array_v) [N], T const& ... valuen_v)
	{
		static auto compare_f = []<auto... I>(auto&& array_v, auto&& valuetup_v, 
			std::index_sequence<I...> index_v)
		{
			return ((array_v[I] == std::get<I>(valuetup_v)) && ...);
		};

		return compare_f(array_v, std::tuple{ valuen_v... }, 
			std::make_index_sequence<sizeof...(T)>{});
	}

};