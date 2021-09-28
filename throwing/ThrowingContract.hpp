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

struct IContract {
  virtual void check_conditions(int filt,
                                std::experimental::source_location const
                                    *const location = nullptr) const = 0;
};

template <bool has_super_contract = false, t_condition... conditions>
struct Contract : public IContract {
  /**
   * @brief Construct a new Throwing Contract object. go over the pre and
   * invariant conditions.
   *
   * @param _location error source_location
   * @param _eg error generator
   * @param _conditions condition list
   */
  Contract(std::experimental::source_location const &_location,
           conditions... _conditions);

  Contract(std::experimental::source_location const &_location,
           IContract const &base, conditions... _conditions);

  /**
   * @brief Destroy the Throwing Contract object. go over the post and invariant
   * conditions.
   */
  ~Contract() noexcept(false);

  // auto const &getConditions() { return m_conditions; };

private:
  /**
   * @brief finds the first condition with the given type and that it's
   * predicate returns false(meaning that there is an error), generate an
   * exception from it and throw the exception.
   *
   * @param filt bitwise type filter.
   */
  void check_conditions(int filt,
                        std::experimental::source_location const
                            *const location = nullptr) const final;

  std::experimental::source_location const &location;
  boost::hana::tuple<conditions...> const m_conditions;
  contain_if<has_super_contract, IContract const &> super;
};

template <t_condition... conditions>
Contract(std::experimental::source_location const &, conditions...)
    -> Contract<false, conditions...>;

template <t_condition... conditions>
Contract(std::experimental::source_location const &, IContract const &,
         conditions...) -> Contract<true, conditions...>;

/*****************IMPLEMENTATION*****************/

template <bool has_super_contract, t_condition... conditions>
Contract<has_super_contract, conditions...>::Contract(
    std::experimental::source_location const &_location,
    conditions... _conditions)
    : location(_location), m_conditions(_conditions...) {
  check_conditions(precondition | invariant);
}

template <bool has_super_contract, t_condition... conditions>
Contract<has_super_contract, conditions...>::Contract(
    std::experimental::source_location const &_location, IContract const &base,
    conditions... _conditions)
    : location(_location), m_conditions(_conditions...), super{base} {
  check_conditions(precondition | invariant);
}

template <bool has_super_contract, t_condition... conditions>
Contract<has_super_contract, conditions...>::~Contract() noexcept(false) {
  check_conditions(postcondition | invariant);
}

template <bool has_super_contract, t_condition... conditions>
void Contract<has_super_contract, conditions...>::check_conditions(
    int filt,
    std::experimental::source_location const *const location) const {
  auto const &where = location ? *location : this->location;

  if constexpr (has_super_contract)
    (*super).check_conditions(filt, &where);

  boost::hana::for_each(m_conditions, [&where, filt](auto const &condition) {
    if (condition.does_violate(filt)) {
      throw condition.getException(where);
    }
  });
}

template <typename T>
concept t_contract = is_same_template<T, Contract<>>;

} // namespace Contract_ns::Throwing

#endif // THROWING_CONTRACT__HPP