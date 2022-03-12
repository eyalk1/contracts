#ifndef UTILITY
#define UTILITY

#include <fmt/core.h>

#include <experimental/source_location>
#include <string_view>
#include <utility>

auto empty = [] {};

auto truer = [] { return true; };

auto falser = [] { return false; };

/****************CONCEPTS****************/

template <typename T>
concept exception = std::is_base_of_v<std::exception, T>;

template <typename T>
concept argumentless_function = requires(T t) {
  std::invocable<T>;
  std::is_same_v<bool, decltype(t)>;
};

template <typename T>
concept enum_t = std::is_enum_v<T>;

template <typename F>
concept descGen = requires(F f) {
  std::is_invocable_v<F, std::experimental::source_location const &,
                      std::string_view>;
  std::is_same_v<std::string, decltype(f)>;
};

/****************IS_SAME_TEMPLATE****************/

template <typename...> constexpr bool is_same_template{false};

template <template <typename...> typename T, typename... A, typename... B>
constexpr bool is_same_template<T<A...>, T<B...>>{true};

/****************MANUAL ERROR DEFAULT****************/

using myError_t = std::pair<int, std::string>;

auto runtime_builder = [](std::experimental::source_location context,
                          std::string_view description, int EC) -> myError_t {
  return {EC, fmt::format("{} {}", description, context.function_name())};
};

/****************CONTAIN IF****************/

template <bool does_contain, typename T> struct contain_if;

template <typename T> struct contain_if<true, T> {
  T what;
  T const &operator*() const { return what; };
};

template <typename T> struct contain_if<false, T> {};

/****************INT COMPARE COMPILE TIME******/

constexpr bool isGt(int i, int j) { return i > j; };

constexpr auto sum(auto i, auto j) { return i + j; };

/****************OVERLOAD CLASS****************/

template <class... Ts> struct overload : Ts... { using Ts::operator()...; };
template <class... Ts> overload(Ts...) -> overload<Ts...>;

/****************Enum Bitmask****************/
template <enum_t Enum> constexpr bool EnableBitMaskOperators = false;

template <enum_t Enum>
requires EnableBitMaskOperators<Enum>
constexpr Enum operator|(Enum lhs, Enum rhs) {
  using underlying = typename std::underlying_type_t<Enum>;
  return static_cast<Enum>(static_cast<underlying>(lhs) |
                           static_cast<underlying>(rhs));
}

template <enum_t Enum>
requires EnableBitMaskOperators<Enum>
constexpr Enum operator&(Enum lhs, Enum rhs) {
  using underlying = typename std::underlying_type_t<Enum>;
  return static_cast<Enum>(static_cast<underlying>(lhs) &
                           static_cast<underlying>(rhs));
}

template <enum_t Enum>
requires EnableBitMaskOperators<Enum>
constexpr Enum operator^(Enum lhs, Enum rhs) {
  using underlying = typename std::underlying_type_t<Enum>;
  return static_cast<Enum>(static_cast<underlying>(lhs) ^
                           static_cast<underlying>(rhs));
}

template <enum_t Enum>
requires EnableBitMaskOperators<Enum>
constexpr Enum operator~(Enum rhs) {
  using underlying = typename std::underlying_type_t<Enum>;
  return static_cast<Enum>(~static_cast<underlying>(rhs));
}

template <enum_t Enum>
requires EnableBitMaskOperators<Enum> Enum &operator|=(Enum &lhs, Enum rhs) {
  using underlying = typename std::underlying_type_t<Enum>;
  lhs = static_cast<Enum>(static_cast<underlying>(lhs) |
                          static_cast<underlying>(rhs));

  return lhs;
}

template <enum_t Enum>
requires EnableBitMaskOperators<Enum> Enum &operator&=(Enum &lhs, Enum rhs) {
  using underlying = typename std::underlying_type_t<Enum>;
  lhs = static_cast<Enum>(static_cast<underlying>(lhs) &
                          static_cast<underlying>(rhs));

  return lhs;
}

template <enum_t Enum>
requires EnableBitMaskOperators<Enum> Enum &operator^=(Enum &lhs, Enum rhs) {
  using underlying = typename std::underlying_type_t<Enum>;
  lhs = static_cast<Enum>(static_cast<underlying>(lhs) ^
                          static_cast<underlying>(rhs));

  return lhs;
}

#endif // UTILITY
