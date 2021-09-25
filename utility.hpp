#ifndef UTILITY
#define UTILITY

#include <fmt/core.h>

#include <experimental/source_location>
#include <string_view>
#include <utility>


auto empty = [] {};

auto truer = [] { return true;};

auto falser = [] {return false;};

template<typename T>
concept exception = std::is_base_of_v<std::exception, T>;

template<typename...>
constexpr bool is_same_template{false};

template<
  template<typename...> typename T,
  typename... A,
  typename... B>
constexpr bool is_same_template<
  T<A...>,
  T<B...>>{true};

using myError_t = std::pair<int, std::string>;

auto runtime_builder = [](std::experimental::source_location context,
                          std::string_view description, int EC) -> myError_t {
  return std::make_pair(EC, fmt::format("{} {}", description, context.function_name()));
};


#endif //UTILITY