#ifndef THROWING_CONDITION__HPP
#define THROWING_CONDITION__HPP

#include "../CommonCondition.hpp"
#include "../utility.hpp"

#include <stdexcept>
#include <string_view>
#include <type_traits>
#include <utility>

namespace Contract_ns::Throwing {

template <typename F> struct Condition {
  constexpr Condition(F _pred, std::string_view _description,
                      int error_code, cond_type _type = precondition);
  int m_error_code;
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
  template <typename new_f>
  Condition<new_f> operator=(Condition<new_f> cond) const;
};

template <typename F>
constexpr Condition<F>::Condition(F _pred, std::string_view _description,
                                  int error_code, cond_type _type)
    : pred(_pred), description(_description), m_type(_type),
      m_error_code(error_code) {}

template <typename F>
template <typename new_f>
Condition<new_f> Condition<F>::operator=(Condition<new_f> cond) const {
  return Condition<new_f>(cond.pred, cond.description, cond.m_error_code,
                          cond.m_type);
}

/*****************UTILITIES*****************/

auto condition_generator = [](cond_type type) {
  return [type](auto pred, std::string_view description, int error_code) {
    return Condition<decltype(pred)>(pred, description, error_code, type);
  };
};

auto pre = condition_generator(precondition);

auto post = condition_generator(postcondition);

auto invar = condition_generator(invariant);

template <typename T>
concept m_condition = is_same_template<T, Condition<decltype(empty)>>;

} // namespace Contract_ns::Throwing

#endif // THROWING_CONDITION__HPP