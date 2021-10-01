#ifndef THROWING_CONTRACT__HPP
#define THROWING_CONTRACT__HPP

#include "IContract.hpp"
#include "ThrowingCondition.hpp"

#include <boost/hana.hpp>
#include <experimental/source_location>
#include <fmt/core.h>
#include <sstream>
#include <string>
// dry contracts + friend activate factory function.
// activate function
// take just tuple for sub contract - i like most - cant be used for more than
// two levels of inheritance
namespace Contract_ns::Throwing {

template <bool has_super_contract = false, t_condition... conditions>
struct DryContract : public IContract {
  /**
   * @brief Construct a new Throwing DryContract object. go over the pre and
   * invariant conditions.
   *
   * @param _location error source_location
   * @param _eg error generator
   * @param _conditions condition list
   */
  DryContract(conditions... _conditions);

  DryContract(IContract const &base, conditions... _conditions);

  /**
   * @brief Destroy the Throwing DryContract object. go over the post and
   * invariant conditions.
   */
  ~DryContract() noexcept(false);

  // auto const &getConditions() { return m_conditions; };

protected:
  /**
   * @brief finds the first condition with the given type and that it's
   * predicate returns false(meaning that there is an error), generate an
   * exception from it and throw the exception.
   *
   * @param filt bitwise type filter.
   */
  void check_conditions(
      int filt, std::experimental::source_location const &location) const final;

private:
  boost::hana::tuple<conditions...> const m_conditions;
  contain_if<has_super_contract, IContract const &> super;
};

/*****************IMPLEMENTATION*****************/

template <bool has_super_contract, t_condition... conditions>
DryContract<has_super_contract, conditions...>::DryContract(
    conditions... _conditions)
    : m_conditions(_conditions...) {}

template <bool has_super_contract, t_condition... conditions>
DryContract<has_super_contract, conditions...>::DryContract(
    IContract const &base, conditions... _conditions)
    : m_conditions(_conditions...), super{base} {}

template <bool has_super_contract, t_condition... conditions>
DryContract<has_super_contract, conditions...>::~DryContract() noexcept(false) {
}

template <bool has_super_contract, t_condition... conditions>
void DryContract<has_super_contract, conditions...>::check_conditions(
    int filt, std::experimental::source_location const &location) const {

  if constexpr (has_super_contract)
    (*super).check_conditions(filt, location);

  boost::hana::for_each(m_conditions, [&location, filt](auto const &condition) {
    if (condition.does_violate(filt)) {
      throw condition.getException(location);
    }
  });
}

template <typename T>
concept t_contract = is_same_template<T, DryContract<>>;

} // namespace Contract_ns::Throwing

#endif // THROWING_CONTRACT__HPP