#ifndef THROWING_CONTRACT__HPP
#define THROWING_CONTRACT__HPP

#include "ThrowingCondition.hpp"

#include <fmt/core.h>
#include <boost/hana.hpp>
#include <experimental/source_location>
#include <sstream>
#include <string>

namespace Contract_ns::Throwing {

#define TCONTRACT(...)                                                         \
  Contract(std::experimental::source_location::current(), __VA_ARGS__)

/**
 * @brief the default exception generator
 *
 * @param loc source_location
 */

std::string GenerateException(std::experimental::source_location loc,
                              std::string_view description);

template <t_condition... conditions>
struct Contract {
  /**
   * @brief Construct a new Throwing Contract object. go over the pre and
   * invariant conditions.
   *
   * @param _location error source_location
   * @param _eg error generator
   * @param _conditions condition list
   */
  Contract(std::experimental::source_location _location,
           conditions... _conditions);
  /**
   * @brief Destroy the Throwing Contract object. go over the post and invariant
   * conditions.
   */
  ~Contract();

private:
  /**
   * @brief finds the first condition with the given type and that it's
   * predicate returns false(meaning that there is an error), generate an
   * exception from it and throw the exception.
   *
   * @param filt bitwise type filter.
   */
  void check_conditions(cond_type_t filt);

  std::experimental::source_location const location;
  boost::hana::tuple<conditions...> const m_conditions;
};

/*****************IMPLEMENTATION*****************/

template <t_condition... conditions>
Contract<conditions...>::Contract(std::experimental::source_location _location,
                                  conditions... _conditions)
    : location(_location), m_conditions(_conditions...) {
  check_conditions(precondition | invariant);
}

template <t_condition... conditions>
Contract<conditions...>::~Contract() {
  check_conditions(postcondition | invariant);
}

template <t_condition... conditions>
void Contract<conditions...>::check_conditions(cond_type_t filt) {
  boost::hana::for_each(m_conditions, [this, filt](auto const &condition) {
    if ((condition.m_type & filt) && !(condition.pred()))
      throw condition.getException(
          GenerateException(this->location, condition.description));
  });
}

std::string GenerateException(std::experimental::source_location loc,
                              std::string_view description) {
  return fmt::format("{}: {}.\n{}: {}",
    "condition not met at the function", loc.function_name(),
    "the error is", description);
}

} // namespace Contract_ns::Throwing

#endif // THROWING_CONTRACT__HPP