#ifndef THROWING_CONDITION__HPP
#define THROWING_CONDITION__HPP

#include "../CommonCondition.hpp"

#include <stdexcept>
#include <string_view>
#include <type_traits>
#include <utility>

namespace Contract_ns::Throwing {

template <typename Exc, typename F> struct Condition {
  constexpr Condition(F _pred, std::string_view _description,
                    cond_type _type = precondition);
  Exc getException(std::string const &description) const;
  F const pred;
  std::string_view const description;
  cond_type m_type;

  /**
   * @brief create and return a condition, override it's type with this's type.
   *
   * @tparam new_f predicate of new condition
   * @param _pred predicate of new condition
   * @return condition<new_f> return the new constructed predicate
   */
  template <typename new_Exc, typename new_f>
  Condition<new_Exc, new_f> operator=(Condition<new_Exc, new_f> cond) const;
};


template <typename Exc, typename F>
constexpr Condition<Exc, F>::Condition(F _pred, std::string_view _description,
                                   cond_type _type)
    : pred(_pred), description(_description), m_type(_type) {}

template <typename Exc, typename F>
Exc Condition<Exc, F>::getException(std::string const &description) const {
  return Exc(description);
};

template <typename Exc, typename F>
template <typename new_Exc, typename new_f>
Condition<new_Exc, new_f>
Condition<Exc, F>::operator=(Condition<new_Exc, new_f> cond) const {
  return Condition<new_Exc, new_f>(cond.pred, cond.description, cond.m_type);
}


/*****************UTILITIES*****************/

/**
 * static objects that give you the ability to write python-like parameter
 * passing
 */
auto empty = [] {};


template<typename T>
concept exception = std::is_base_of_v<std::exception, T>;

template<exception E>
auto pre(auto pred, std::string_view description){
  return Condition<E, decltype(pred)>(pred, description, precondition);
}

template<exception E>
auto post(auto pred, std::string_view description){
  return Condition<E, decltype(pred)>(pred, description, postcondition);
}

template<exception E>
auto invar(auto pred, std::string_view description){
  return Condition<E, decltype(pred)>(pred, description, invariant);
}


template<typename...>
constexpr bool is_same_template{false};

template<
  template<typename...> typename T,
  typename... A,
  typename... B>
constexpr bool is_same_template<
  T<A...>,
  T<B...>>{true};

template <typename T>
concept t_condition = is_same_template<T, Condition<decltype(empty), int>>;


} // namespace Contract_ns::Throwing

#endif // THROWING_CONDITION__HPP