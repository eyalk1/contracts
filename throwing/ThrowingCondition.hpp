#ifndef THROWING_CONDITION__HPP
#define THROWING_CONDITION__HPP

#include "../CommonCondition.hpp"
#include "../utility.hpp"

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

template<exception E>
auto condition_generator = [] (cond_type type) {
  return [type] (auto pred, std::string_view description) {
    return Condition<E, decltype(pred)>(pred, description, type);
  };
};

template<exception E>
auto pre = condition_generator<E>(precondition);

template<exception E>
auto post = condition_generator<E>(postcondition);

template<exception E>
auto invar = condition_generator<E>(invariant);

template <typename T>
concept t_condition = is_same_template<T, Condition<decltype(empty), int>>;


} // namespace Contract_ns::Throwing

#endif // THROWING_CONDITION__HPP