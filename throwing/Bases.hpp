#ifndef BASE_COLLECTION__HPP
#define BASE_COLLECTION__HPP

#include "IContract.hpp"
#include "../utility.hpp"
#include <concepts>
namespace Contract_ns::Throwing {

template <std::integral auto num_of_supers> struct Bases {
  template <typename... things>
  Bases(things const &...thing)
      : b{std::reference_wrapper<IContract const>(thing)...} {};

  using arr_t =
      std::array<std::reference_wrapper<IContract const>, num_of_supers>;

  arr_t b;

  constexpr typename arr_t::size_type size() { return b.size(); }
};

template <typename... things>
Bases(things const &... thing) -> Bases<sizeof...(things)>;

template <typename T>
concept base_t = is_same_template<T, Bases<0>>;
}// namespace Contract_ns::Throwing

#endif // BASE_COLLECTION__HPP