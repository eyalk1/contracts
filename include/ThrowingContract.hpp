#ifndef THROWING_CONTRACT__HPP
#define THROWING_CONTRACT__HPP

#include "ThrowingCondition.hpp"

#include <boost/hana.hpp>
#include <experimental/source_location>
#include <sstream>
#include <stdexcept>
#include <string>

#define TCONTRACT(...)                                                      \
  ThrowingContract(std::experimental::source_location::current(), __VA_ARGS__)

/**
 * @brief the default exception generator
 *
 * @param loc source_location
 */

std::string GenerateException(std::experimental::source_location loc,
                              std::string_view description);

template <t_condition... conditions> struct ThrowingContract {
  /**
   * @brief Construct a new Throwing Contract object. go over the pre and
   * invariant conditions.
   *
   * @param _location error source_location
   * @param _eg error generator
   * @param _conditions condition list
   */
  ThrowingContract(std::experimental::source_location _location,
                   conditions... _conditions);
  /**
   * @brief Destroy the Throwing Contract object. go over the post and invariant
   * conditions.
   */
  ~ThrowingContract();

private:
  /**
   * @brief finds the first condition with the given type and that it's
   * predicate returns false(meaning that there is an error), generate an
   * exception from it and throw the exception.
   *
   * @param filt bitwise type filter.
   */
  void go_over(cond_type_t filt);

  std::experimental::source_location location;
  boost::hana::tuple<conditions...> const m_conditions;
};

/*****************IMPLEMENTATION*****************/

template <t_condition... conditions>
ThrowingContract<conditions...>::ThrowingContract(
    std::experimental::source_location _location, conditions... _conditions)
    : m_conditions(_conditions...), location(_location) {
  go_over(precondition | invariant);
}

template <t_condition... conditions>
ThrowingContract<conditions...>::~ThrowingContract() {
  go_over(postcondition | invariant);
}

template <t_condition... conditions>
void ThrowingContract<conditions...>::go_over(cond_type_t filt) {
  boost::hana::for_each(m_conditions, [this, filt](auto const &condition) {
    // std::cout << "in loop filter " << filt << " condition.m_type "
    //           << condition.m_type << " condition.cond.description "
    //           << condition.cond.description << " condition.cond.pred() "
    //           << condition.cond.pred() << '\n';
    if ((condition.m_type & filt) && !(condition.cond.pred()))
      throw condition.getException(
          GenerateException(this->location, condition.cond.description));
  });
}

std::string GenerateException(std::experimental::source_location loc,
                              std::string_view description) {
  std::stringstream ss;
  ss << "condition not met at entrance to the function " << loc.function_name()
     << " .the error is: " << description;
  return ss.str();
}

#endif // THROWING_CONTRACT__HPP