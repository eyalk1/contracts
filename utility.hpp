#ifndef UTILITY
#define UTILITY

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


#endif //UTILITY
