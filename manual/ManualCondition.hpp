#ifndef MANUAL_CONDITION__HPP
#define MANUAL_CONDITION__HPP

#include "CommonCondition.hpp"

#include <string_view>
#include <utility>

namespace Contract_ns::Manual {

template <typename F> struct Condata {
  constexpr Condata(F _f, std::string_view _description, int _error_code)
      : pred(_f), description(_description), error_code(_error_code) {}
  F const pred;
  std::string_view const description;
  int error_code;
};

/**
 * @brief the condition class.
 * this class holds a predicate to check, and a description of the error upon
 * violating the predicate. the predicate may return:
 * 1. true or false in the case of Throwing contracts
 * 2. the Error code in the case of manual error handling contract
 *
 * @tparam F the predicate.
 */
template <typename F> struct Condition {
  /**
   * @brief Construct a new condition object.
   *
   * @param _type the type - precondition, invariant, postcondition
   * @param _pred the predicate to check
   * @param _description the error to inform
   */
  constexpr Condition(cond_type _type, F _pred, std::string_view _description,
                      int _error_code);

  constexpr Condition(cond_type _type, Condata<F> _data);

  /**
   * @brief create and return a condition, override it's type with this's type.
   *
   * @tparam new_f predicate of new condition
   * @param _pred predicate of new condition
   * @return condition<new_f> return the new constructed predicate
   */
  template <typename new_f>
  Condition<new_f> operator=(Condata<new_f> _pred) const;

  template <typename new_f>
  Condition<new_f> operator=(Condition<new_f> rhs) const;

  cond_type m_type;
  Condata<F> const cond;
  //   F const pred;
  //   std::string_view const description;
  //   int error_code;
};

template <typename> struct is_m_condition {
  static constexpr auto value = false;
};
template <typename T> struct is_m_condition<Condition<T>> {
  static constexpr auto value = true;
};

template <typename T>
concept m_condition = is_m_condition<T>::value;

/*****************IMPLEMENTATION*****************/

template <typename F>
constexpr Condition<F>::Condition(cond_type _type, F _pred,
                                  std::string_view _description,
                                  int _error_code)
    : m_type(_type), cond(_pred, _description, _error_code){};

template <typename F>
constexpr Condition<F>::Condition(cond_type _type, Condata<F> _data)
    : cond(_data), m_type(_type) {}

template <typename F>
template <typename new_f>
Condition<new_f> Condition<F>::operator=(Condata<new_f> _pred) const {
  return Condition<new_f>(this->m_type, _pred);
}

template <typename F>
template <typename new_f>
Condition<new_f> Condition<F>::operator=(Condition<new_f> rhs) const {
  rhs.m_type = this->m_type;
  return std::move(rhs);
}

/*****************UTILITIES*****************/

/**
 * static objects that give you the ability to write python-;like parameter
 * passing
 */
static constexpr Condition pre(
    precondition, []() {}, "", 0);
static constexpr Condition invar(
    invariant, [] {}, "", 0);
static constexpr Condition post(
    postcondition, [] {}, "", 0);

} // namespace Contract_ns::Manual

#endif // MANUAL_CONDITION__HPP