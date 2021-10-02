#ifndef THROWING_CONTRACT__HPP
#define THROWING_CONTRACT__HPP

#include "IContract.hpp"
#include "ThrowingCondition.hpp"

#include <boost/hana.hpp>
#include <experimental/source_location>
#include <fmt/core.h>
#include <sstream>
#include <string>
#include <functional>
#include <vector>

namespace Contract_ns::Throwing {
using super_list = std::vector<std::reference_wrapper<IContract const>>;
using super_init_list = std::initializer_list<std::reference_wrapper<IContract const>>;

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

  DryContract(super_init_list bases, conditions... _conditions);

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
  contain_if<has_super_contract, super_list> supers;
};

/*****************IMPLEMENTATION*****************/

template <bool has_super_contract, t_condition... conditions>
DryContract<has_super_contract, conditions...>::DryContract(
    conditions... _conditions)
    : m_conditions(_conditions...) {}

template <bool has_super_contract, t_condition... conditions>
DryContract<has_super_contract, conditions...>::DryContract(
    super_init_list base, conditions... _conditions)
    : m_conditions(_conditions...), supers{base} {}

template <bool has_super_contract, t_condition... conditions>
DryContract<has_super_contract, conditions...>::~DryContract() noexcept(false) {
}

template <bool has_super_contract, t_condition... conditions>
void DryContract<has_super_contract, conditions...>::check_conditions(
    int filt, std::experimental::source_location const &location) const {
  // if we are already in an exception, we don't need to add another error on top of it.
  if(std::uncaught_exceptions())
    return;

  if constexpr (has_super_contract)
    for(auto const & super : *supers)
      super.get().check_conditions(filt, location);

  boost::hana::for_each(m_conditions, [&location, filt](auto const &condition) {
    if (condition.is_violated(filt)) {
      throw condition.getException(location);
    }
  });
}

template <typename T>
concept t_contract = is_same_template<T, DryContract<>>;

} // namespace Contract_ns::Throwing

#endif // THROWING_CONTRACT__HPP