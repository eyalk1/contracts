#ifndef CONDITION__HPP
#define CONDITION__HPP

#include <optional>
#include <string_view>
#include <utility>

enum cond_type { PRE = 1, INVARIANT = 2, POST = 4 };

using cond_type_t = std::underlying_type_t<cond_type>;

/**
 * @brief the condition class.
 * this class holds a predicate to check, and a description of the error upon
 * violating the predicate. the predicate may return:
 * 1. true or false in the case of Throwing contracts
 * 2. the Error code in the case of manual error handling contract
 *
 * @tparam F the predicate.
 */
template <typename F> struct condition {
  /**
   * @brief Construct a new condition object.
   *
   * @param _type the type - precondition, invariant, postcondition
   * @param _pred the predicate to check
   * @param _description the error to inform
   */
  constexpr condition(cond_type _type, F _pred, std::string_view _description);

  /**
   * @brief create and return a condition, override it's type with this's type.
   *
   * @tparam new_f predicate of new condition
   * @param _pred predicate of new condition
   * @return condition<new_f> return the new constructed predicate
   */
  template <typename new_f>
  condition<new_f> operator=(std::pair<new_f, char const *> _pred) const;

  template <typename new_f>
  condition<new_f> operator=(condition<new_f> rhs) const;

  cond_type m_type;
  F const pred;
  std::string_view const description;
};


template<typename>
struct is_condition{static constexpr auto value = false;};
template<typename T>
struct is_condition<condition<T>>{static constexpr auto value = true;};

template<typename T>
concept condition_t = is_condition<T>::value;


/*****************IMPLEMENTATION*****************/

template <typename F>
constexpr condition<F>::condition(cond_type _type, F _pred,
                                  std::string_view _description)
    : m_type(_type), pred(_pred), description(_description){};

template <typename F>
template <typename new_f>
condition<new_f>
condition<F>::operator=(std::pair<new_f, char const *> _pred) const {
  return condition<new_f>(this->m_type, _pred.first, _pred.second);
}

template <typename F>
template <typename new_f>
condition<new_f> condition<F>::operator=(condition<new_f> rhs) const {
  rhs.m_type = this->m_type;
  return std::move(rhs);
}

/*****************UTILITIES*****************/

/**
 * makes it a bit less tedious to write conditions.
 */
static constexpr auto precondition = cond_type::PRE;
static constexpr auto invariant = cond_type::INVARIANT;
static constexpr auto postcondition = cond_type::POST;

/**
 * static objects that give you the ability to write python-;like parameter
 * passing
 */
static constexpr condition pre(
    precondition, [] {}, "");
static constexpr condition invar(
    invariant, [] {}, "");
static constexpr condition post(
    postcondition, [] {}, "");
#endif // CONDITION__HPP