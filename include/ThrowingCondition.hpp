#ifndef THROWING_CONDITION__HPP
#define THROWING_CONDITION__HPP

#include "CommonCondition.hpp"

#include <stdexcept>
#include <string_view>
#include <type_traits>
#include <utility>

namespace Contract::Throwing {

template <typename F, typename Exc> struct ThrowingCondition;

template <typename F, typename Exc = void> struct ThrowingCondData {
  constexpr ThrowingCondData(F _pred, std::string_view _description,
                             cond_type _type);
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
  template <typename new_f, typename new_Exc>
  ThrowingCondition<new_f, Exc>
  operator=(ThrowingCondData<new_f, new_Exc> cond) const;
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
template <typename F, typename Exc> struct ThrowingCondition {
  /**
   * @brief Construct a new condition object.
   *
   * @param _type the type - precondition, invariant, postcondition
   * @param _pred the predicate to check
   * @param _description the error to inform
   */
  constexpr ThrowingCondition(cond_type _type, F _pred,
                              std::string_view _description);
  constexpr ThrowingCondition(ThrowingCondData<F, Exc> _cond);

  template <typename new_f, typename new_Exc>
  ThrowingCondition<new_f, new_Exc>
  operator=(ThrowingCondition<new_f, new_Exc> rhs) const;

  ThrowingCondData<F, Exc> cond;
};

template <typename> struct is_condition {
  static constexpr auto value = false;
};
template <typename T, typename Exc>
struct is_condition<ThrowingCondition<T, Exc>> {
  static constexpr auto value = true;
};

template <typename T>
concept t_condition = is_condition<T>::value;

/*****************IMPLEMENTATION*****************/

template <typename F, typename Exc>
constexpr ThrowingCondition<F, Exc>::ThrowingCondition(
    cond_type _type, F _pred, std::string_view _description)
    : cond(_pred, _description, _type){};

template <typename F, typename Exc>
constexpr ThrowingCondition<F, Exc>::ThrowingCondition(
    ThrowingCondData<F, Exc> _cond)
    : cond(_cond){};

template <typename F, typename Exc>
template <typename new_f, typename new_Exc>
ThrowingCondition<new_f, Exc> ThrowingCondData<F, Exc>::operator=(
    ThrowingCondData<new_f, new_Exc> cond) const {
  return ThrowingCondition<new_f, Exc>(cond.m_type, cond.pred, cond.description);
}

template <typename F, typename Exc>
template <typename new_f, typename new_Exc>
ThrowingCondition<new_f, new_Exc> ThrowingCondition<F, Exc>::operator=(
    ThrowingCondition<new_f, new_Exc> rhs) const {
  rhs.m_type = this->m_type;
  return std::move(rhs);
}

template <typename F, typename Exc>
constexpr ThrowingCondData<F, Exc>::ThrowingCondData(
    F _pred, std::string_view _description, cond_type _type)
    : pred(_pred), description(_description), m_type(_type) {}

template <typename F, typename Exc>
Exc ThrowingCondData<F, Exc>::getException(
    std::string const &description) const {
  return Exc(description);
};

/*****************UTILITIES*****************/

/**
 * static objects that give you the ability to write python-like parameter
 * passing
 */
auto empty = [] {};

template <typename Error>
static constexpr ThrowingCondData<decltype(empty), Error> pre(empty, "", precondition);

template <typename Error>
static constexpr ThrowingCondData<decltype(empty), Error> invar(empty, "", invariant);

template <typename Error>
static constexpr ThrowingCondData<decltype(empty), Error> post(empty, "", postcondition);

} // namespace Contract::Throwing

#endif // THROWING_CONDITION__HPP