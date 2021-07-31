#ifndef MANUAL_CONDITION__HPP
#define MANUAL_CONDITION__HPP

#include "CommonCondition.hpp"

#include <optional>
#include <string_view>
#include <utility>


using cond_type_t = std::underlying_type_t<cond_type>;

template<typename F>
struct ManualCondData
{
    constexpr ManualCondData(F _f, std::string_view _description, int _error_code):
        pred(_f), description(_description), error_code(_error_code){}
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
template <typename F> struct ManualCondition {
  /**
   * @brief Construct a new condition object.
   *
   * @param _type the type - precondition, invariant, postcondition
   * @param _pred the predicate to check
   * @param _description the error to inform
   */
  constexpr ManualCondition(cond_type _type, F _pred,
                            std::string_view _description, int _error_code);

  constexpr ManualCondition(cond_type _type, ManualCondData<F> _data);

  /**
   * @brief create and return a condition, override it's type with this's type.
   *
   * @tparam new_f predicate of new condition
   * @param _pred predicate of new condition
   * @return condition<new_f> return the new constructed predicate
   */
  template <typename new_f>
  ManualCondition<new_f>
  operator=(ManualCondData<new_f> _pred) const;

  template <typename new_f>
  ManualCondition<new_f> operator=(ManualCondition<new_f> rhs) const;

  cond_type m_type;
  ManualCondData<F> const cond;
//   F const pred;
//   std::string_view const description;
//   int error_code;
};

template <typename> struct is_m_condition {
  static constexpr auto value = false;
};
template <typename T> struct is_m_condition<ManualCondition<T>> {
  static constexpr auto value = true;
};

template <typename T>
concept m_condition = is_m_condition<T>::value;

/*****************IMPLEMENTATION*****************/

template <typename F>
constexpr ManualCondition<F>::ManualCondition(cond_type _type, F _pred,
                                              std::string_view _description,
                                              int _error_code)
    : m_type(_type), cond(_pred, _description,_error_code){};

template <typename F>
constexpr ManualCondition<F>::ManualCondition(cond_type _type, ManualCondData<F> _data):
    cond(_data), m_type(_type){}

template <typename F>
template <typename new_f>
ManualCondition<new_f> ManualCondition<F>::operator=(
    ManualCondData<new_f> _pred) const {
  return ManualCondition<new_f>(this->m_type, _pred);
}

template <typename F>
template <typename new_f>
ManualCondition<new_f>
ManualCondition<F>::operator=(ManualCondition<new_f> rhs) const {
  rhs.m_type = this->m_type;
  return std::move(rhs);
}

/*****************UTILITIES*****************/

/**
 * static objects that give you the ability to write python-;like parameter
 * passing
 */
static constexpr ManualCondition pre_m(
    precondition, [](){}, "", 0);
static constexpr ManualCondition invar_m(
    invariant, [] {}, "", 0);
static constexpr ManualCondition post_m(
    postcondition, [] {}, "", 0);
#endif // MANUAL_CONDITION__HPP