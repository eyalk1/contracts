#ifndef THROWING_CONTRACT__HPP
#define THROWING_CONTRACT__HPP

#include "condition.hpp"

#include <boost/hana.hpp>
#include <experimental/source_location>
#include <sstream>
#include <stdexcept>

#define TCONTRACT(...)                                                         \
  ThrowingContract(std::experimental::source_location::current(),              \
                   &GenerateException, __VA_ARGS__)
#define TCONTRACT_EG(...)                                                      \
  ThrowingContract(std::experimental::source_location::current(), __VA_ARGS__)


/**
 * @brief the default exception generator
 *
 * @param loc source_location
 */
std::runtime_error GenerateException(std::experimental::source_location loc,
                                     std::string_view description);


template <typename ExceptionGen, condition_t... conditions>
struct ThrowingContract {
  /**
   * @brief Construct a new Throwing Contract object. go over the pre and
   * invariant conditions.
   *
   * @param _location error source_location
   * @param _eg error generator
   * @param _conditions condition list
   */
  ThrowingContract(std::experimental::source_location _location,
                   ExceptionGen _eg, conditions... _conditions);
  /**
   * @brief Destroy the Throwing Contract object. go over the post and invariant
   * conditions.
   */
  ~ThrowingContract();

private:
/**
 * @brief finds the first condition with the given type and that it's predicate
 * returns false(meaning that there is an error), generate an exception from it
 * and throw the exception.
 *
 * @param filt bitwise type filter.
 */
  void go_over(cond_type_t filt);

  ExceptionGen eg;
  std::experimental::source_location location;
  boost::hana::tuple<conditions...> const m_conditions;
};

/*****************IMPLEMENTATION*****************/

template <typename ExceptionGen, condition_t... conditions>
ThrowingContract<ExceptionGen, conditions...>::ThrowingContract(
    std::experimental::source_location _location, ExceptionGen _eg,
    conditions... _conditions)
    : m_conditions(_conditions...), location(_location), eg(_eg) {
  go_over(precondition | invariant);
}

template <typename ExceptionGen, condition_t... conditions>
ThrowingContract<ExceptionGen, conditions...>::~ThrowingContract() {
  go_over(postcondition | invariant);
}

template <typename ExceptionGen, condition_t... conditions>
void ThrowingContract<ExceptionGen, conditions...>::go_over(cond_type_t filt) {
  boost::hana::for_each(m_conditions, [this, filt](auto const &condition) {
    if ((condition.m_type & filt) && !(condition.pred()))
      throw eg(this->location, condition.description);
  });
}


std::runtime_error GenerateException(std::experimental::source_location loc,
                                     std::string_view description) {
  std::stringstream ss;
  ss << "condition not met at entrance to the function " << loc.function_name()
     << " .the error is: " << description;
  return std::runtime_error(ss.str());
}

#endif // THROWING_CONTRACT__HPP