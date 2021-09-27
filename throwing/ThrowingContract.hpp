#ifndef THROWING_CONTRACT__HPP
#define THROWING_CONTRACT__HPP

#include "ThrowingCondition.hpp"

#include <boost/hana.hpp>
#include <experimental/source_location>
#include <fmt/core.h>
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

std::string GenerateException(std::experimental::source_location const& loc,
                              std::string_view description);

struct IContract {
  virtual void check_conditions(cond_type_t filt, std::experimental::source_location const * const location = nullptr) const = 0;
};

template <bool has_base = false, t_condition... conditions>
struct Contract : public IContract {
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

  Contract(std::experimental::source_location _location, IContract *base,
           conditions... _conditions);

  /**
   * @brief Destroy the Throwing Contract object. go over the post and invariant
   * conditions.
   */
  ~Contract();

  // auto const &getConditions() { return m_conditions; };

private:
  /**
   * @brief finds the first condition with the given type and that it's
   * predicate returns false(meaning that there is an error), generate an
   * exception from it and throw the exception.
   *
   * @param filt bitwise type filter.
   */
  void check_conditions(
      cond_type_t filt,
      std::experimental::source_location const * const location = nullptr) const final;

  std::experimental::source_location const location;
  boost::hana::tuple<conditions...> const m_conditions;
  contain_if<has_base, IContract const *> base;
};

template <t_condition... conditions>
Contract(std::experimental::source_location, conditions...)
    -> Contract<false, conditions...>;

template <t_condition... conditions>
Contract(std::experimental::source_location, IContract *, conditions...)
    -> Contract<true, conditions...>;

/*****************IMPLEMENTATION*****************/

template <bool has_base, t_condition... conditions>
Contract<has_base, conditions...>::Contract(
    std::experimental::source_location _location, conditions... _conditions)
    : location(_location), m_conditions(_conditions...) {
  check_conditions(precondition | invariant);
}

template <bool has_base, t_condition... conditions>
Contract<has_base, conditions...>::Contract(
    std::experimental::source_location _location, IContract *base,
    conditions... _conditions)
    : location(_location), m_conditions(_conditions...), base{base} {
  check_conditions(precondition | invariant);
}

template <bool has_base, t_condition... conditions>
Contract<has_base, conditions...>::~Contract() {
  check_conditions(postcondition | invariant);
}

template <bool has_base, t_condition... conditions>
void Contract<has_base, conditions...>::check_conditions(
    cond_type_t filt, std::experimental::source_location const * const location) const {
  auto const &where = location ? *location : this->location;

  if constexpr (has_base)
    base.what->check_conditions(filt, &where);


  boost::hana::for_each(m_conditions, [&where, filt](auto const &condition) {
    if ((condition.m_type & filt) && !(condition.pred())) {
      throw condition.getException(
          GenerateException(where, condition.description));
    }
  });
}

std::string GenerateException(std::experimental::source_location const& loc,
                              std::string_view description) {
  return fmt::format("{}: {}.\n{}: {}.\n{}: {}",
                      "condition not met at the function", loc.function_name(),
                      "the error is", description,
                      "line: ", loc.line());
}

template <typename T>
concept t_contract = is_same_template<T, Contract<>>;

} // namespace Contract_ns::Throwing

#endif // THROWING_CONTRACT__HPP