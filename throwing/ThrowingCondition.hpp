#ifndef THROWING_CONDITION__HPP
#define THROWING_CONDITION__HPP

#include "../CommonCondition.hpp"

#include <stdexcept>
#include <string_view>
#include <type_traits>
#include <utility>

namespace Contract_ns::Throwing {

template <typename F, typename Exc> struct Condition;

template <typename F, typename Exc = void> struct Condata {
  constexpr Condata(F _pred, std::string_view _description,
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
  template <typename new_f, typename new_Exc>
  Condition<new_f, Exc> operator=(Condata<new_f, new_Exc> cond) const;
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
template <typename F, typename Exc> struct Condition {
  /**
   * @brief Construct a new condition object.
   *
   * @param _type the type - precondition, invariant, postcondition
   * @param _pred the predicate to check
   * @param _description the error to inform
   */
  constexpr Condition(cond_type _type, F _pred, std::string_view _description);
  constexpr Condition(Condata<F, Exc> _cond);

  template <typename new_f, typename new_Exc>
  Condition<new_f, new_Exc> operator=(Condition<new_f, new_Exc> rhs) const;

  Condata<F, Exc> m_cond;
};

/*****************IMPLEMENTATION*****************/

template <typename F, typename Exc>
constexpr Condition<F, Exc>::Condition(cond_type _type, F _pred,
                                       std::string_view _description)
    : m_cond(_pred, _description, _type){};

template <typename F, typename Exc>
constexpr Condition<F, Exc>::Condition(Condata<F, Exc> _cond) : m_cond(_cond){};

template <typename F, typename Exc>
template <typename new_f, typename new_Exc>
Condition<new_f, Exc>
Condata<F, Exc>::operator=(Condata<new_f, new_Exc> cond) const {
  return Condition<new_f, Exc>(cond.m_type, cond.pred, cond.description);
}

template <typename F, typename Exc>
template <typename new_f, typename new_Exc>
Condition<new_f, new_Exc>
Condition<F, Exc>::operator=(Condition<new_f, new_Exc> rhs) const {
  rhs.m_type = this->m_type;
  return std::move(rhs);
}

template <typename F, typename Exc>
constexpr Condata<F, Exc>::Condata(F _pred, std::string_view _description,
                                   cond_type _type)
    : pred(_pred), description(_description), m_type(_type) {}

template <typename F, typename Exc>
Exc Condata<F, Exc>::getException(std::string const &description) const {
  return Exc(description);
};

/*****************UTILITIES*****************/

/**
 * static objects that give you the ability to write python-like parameter
 * passing
 */
auto empty = [] {};

template <typename Error>
static constexpr Condata<decltype(empty), Error> pre(empty, "", precondition);

template <typename Error>
static constexpr Condata<decltype(empty), Error> invar(empty, "", invariant);

template <typename Error>
static constexpr Condata<decltype(empty), Error> post(empty, "", postcondition);

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


// template <typename T>
// concept t_condition = is_condition_data<T>::value;Condata

} // namespace Contract_ns::Throwing

#endif // THROWING_CONDITION__HPP