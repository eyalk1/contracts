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

template<typename T>
concept argumentless_function =
  requires(T t){
    std::invocable<T>;
    std::is_same_v<bool, decltype(t)>;
  };

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
  return {EC, fmt::format("{} {}", description, context.function_name())};
};

template<bool does_contain, typename T>
struct contain_if;

template<typename T>
struct contain_if<true, T>{
  T what;
  // T& operator*() const { return what;};
  T const& operator*() const { return what;};
};

template<typename T>
struct contain_if<false, T>{
};

template<class... Ts> struct overload : Ts... {using Ts::operator()...;};
template<class... Ts> overload(Ts...) -> overload<Ts...>;


#endif //UTILITY
