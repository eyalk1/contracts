#ifndef MANUAL_CONTRACT__HPP
#define MANUAL_CONTRACT__HPP
#include "ManualCondition.hpp"

#include <boost/hana.hpp>
#include <experimental/source_location>
#include <type_traits>

namespace Contract_ns::Manual {

/**
 * @brief A contract class with manual error control.
 *
 * @tparam Error_t the error type
 * @tparam ErrorGenerator_t a function that can generate an Error_t given:
 * 1. source_location
 * 2. error description provided by the corresponding condition object
 * 3. error code provided by the corresponding condition object
 * @tparam conditions
 */
template <typename ErrorGenerator_t, m_condition... conditions>
struct Contract {

  using Error_t = std::result_of_t<ErrorGenerator_t(
      std::experimental::source_location, std::string_view, int)>;

  Contract(ErrorGenerator_t _f, conditions... _conditions);

  /**
   * @brief check for errors.
   *
   * @param check the type of condition to check.
   * @param loc the source_location of the error.
   * @return true there is no error.
   * @return false there is an error.
   */
  bool check(cond_type_t check,
             std::experimental::source_location loc =
                 std::experimental::source_location::current());

  Error_t m_error;

private:
  /**
   * @brief check for an error and sets the m_error member if there is an error.
   *
   * @param to_check the type of condition to check.
   * @param loc the source_location of the error.
   * @return true there is no error.
   * @return false there is an error.
   */
  bool calculateError(cond_type_t to_check,
                      std::experimental::source_location loc);

  ErrorGenerator_t m_predicate;
  boost::hana::tuple<conditions...> const m_conditions;
};

/*****************IMPLEMENTATION*****************/

/**
 * @brief Construct a new Contract object
 *
 * @param _f the function object.
 * @param _conditions the list of conditions for this contract.
 */
template <typename ErrorGenerator_t, m_condition... conditions>
Contract<ErrorGenerator_t, conditions...>::Contract(
    ErrorGenerator_t _f, conditions... _conditions)
    : m_conditions(_conditions...), m_predicate(_f) {
  // static_assert(is_same_template_v<conditions, condition> && ... );
}

template <typename ErrorGenerator_t, m_condition... conditions>
bool Contract<ErrorGenerator_t, conditions...>::check(
    cond_type_t check, std::experimental::source_location loc) {
  return calculateError(check, std::move(loc));
}

template <typename ErrorGenerator_t, m_condition... conditions>
bool Contract<ErrorGenerator_t, conditions...>::calculateError(
    cond_type_t to_check, std::experimental::source_location loc) {
  bool erred_yet{false};
  std::string_view description;
  int EC{0};

  // go over the conditions and find conditions that the concerning predicate
  // erred.
  boost::hana::for_each(m_conditions, [to_check, &erred_yet, &description,
                                       &EC](auto const &condition) {
    if (!erred_yet && (condition.m_type & to_check) &&
        (!condition.cond.pred())) {
      erred_yet = true;

      description = condition.cond.description;
      EC = condition.cond.error_code;
    }
  });

  if (!erred_yet)
    return true;

  m_error = m_predicate(loc, description, EC);
  return false;
}

} // namespace Contract::Manual

#endif // MANUAL_CONTRACT__HPP