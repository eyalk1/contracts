#ifndef MANUAL_CONTRACT__HPP
#define MANUAL_CONTRACT__HPP

#include "ManualCondition.hpp"

#include <boost/hana.hpp>
#include <experimental/source_location>
#include <fmt/core.h>
#include <type_traits>

namespace Contract_ns::Manual {

auto defaultErrorGenerator = [](auto loc, auto description, auto EC) {
  return fmt::format("{}\n{}\n{}", loc.function_name, description, EC);
};

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
struct DryContract {

  using Error_t = std::result_of_t<ErrorGenerator_t(
      std::experimental::source_location, std::string_view, int)>;
  using maybeError = std::optional<Error_t>;

  DryContract(ErrorGenerator_t _f, conditions... _conditions);

  /**
   * @brief check for errors.
   *
   * @param check the type of condition to check.
   * @param loc the source_location of the error.
   * @return true there is no error.
   * @return false there is an error.
   */
  auto check(cond_type_t check,
             std::experimental::source_location loc =
                 std::experimental::source_location::current()) -> maybeError;

private:
  /**
   * @brief check for an error and sets the m_error member if there is an error.
   *
   * @param to_check the type of condition to check.
   * @param loc the source_location of the error.
   * @return true there is no error.
   * @return false there is an error.
   */
  // bool calculateError(cond_type_t to_check,
  //                     std::experimental::source_location loc);

  ErrorGenerator_t const generateError;
  boost::hana::tuple<conditions...> const m_conditions;
};

/*****************IMPLEMENTATION*****************/

/**
 * @brief Construct a new DryContract object
 *
 * @param _f the function object.
 * @param _conditions the list of conditions for this contract.
 */
template <typename ErrorGenerator_t, m_condition... conditions>
DryContract<ErrorGenerator_t, conditions...>::DryContract(ErrorGenerator_t _f,
                                                    conditions... _conditions)
    : generateError(_f), m_conditions(_conditions...) {
}

template <typename ErrorGenerator_t, m_condition... conditions>
auto DryContract<ErrorGenerator_t, conditions...>::check(
    cond_type_t to_check, std::experimental::source_location loc)
    -> DryContract<ErrorGenerator_t, conditions...>::maybeError {
  bool erred_yet{false};
  std::string_view description;
  int EC{0};

  // go over the conditions and find conditions that the concerning predicate
  // erred.
  // cannot be switched into find_if since it:
  //  outputs compile time data(the element found in the tuple)
  //  at run-time.
  boost::hana::for_each(
      m_conditions, [to_check, &erred_yet, &description, &EC](auto const &condition) {
        if (!erred_yet && (condition.m_type & to_check) && (!condition.pred())) {
          erred_yet = true;

          description = condition.description;
          EC = condition.m_error_code;
          return true;
        }
        return false;
      });

  if (!erred_yet)
    return std::nullopt;

  return generateError(loc, description, EC);
}

} // namespace Contract_ns::Manual

#endif // MANUAL_CONTRACT__HPP