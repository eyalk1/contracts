#ifndef THROWING_CONTRACT__HPP
#define THROWING_CONTRACT__HPP

#include "IContract.hpp"
#include "ThrowingCondition.hpp"

#include "Bases.hpp"

#include <boost/hana.hpp>
#include <experimental/source_location>
#include <fmt/core.h>
#include <functional>
#include <sstream>
#include <string>
#include <vector>

namespace Contract_ns::Throwing {
template <std::integral auto num_of_supers>
using super_list_t =
    std::array<std::reference_wrapper<IContract const>, num_of_supers>;

template <std::integral auto num_of_supers = 0, t_condition... conditions>
struct DryContract : public IContract {
  using super_list = super_list_t<num_of_supers>;
  /**
   * @brief Construct a new Throwing DryContract object. go over the pre and
   * invariant conditions.
   *
   * @param _location error source_location
   * @param _eg error generator
   * @param _conditions condition list
   */
  DryContract(conditions... _conditions);

  DryContract(Bases<num_of_supers> &&Bases, conditions... _conditions);

  /**
   * @brief Destroy the Throwing DryContract object. go over the post and
   * invariant conditions.
   */
  ~DryContract() noexcept(false);

  DryContract(DryContract const&) = delete;
  DryContract(DryContract &&) = delete;
  auto operator=(DryContract const&) = delete;
  auto operator=(DryContract &&) = delete;

protected:
  /**
   * @brief finds the first condition with the given type and that it's
   * predicate returns false(meaning that there is an error), generate an
   * exception from it and throw the exception.
   *
   * @param filt bitwise type filter.
   */
  void check_conditions(
      cond_type filt,
      std::experimental::source_location const &location) const final;

private:
  boost::hana::tuple<conditions...> const m_conditions;
  contain_if<isGt(num_of_supers, 0), super_list> supers;
};

template <base_t sl, t_condition... conditions>
DryContract(sl &&, conditions...) -> DryContract<sl::size, conditions...>;

template <t_condition... conditions>
DryContract(conditions...) -> DryContract<0, conditions...>;

/*****************IMPLEMENTATION*****************/

template <std::integral auto num_of_supers, t_condition... conditions>
DryContract<num_of_supers, conditions...>::DryContract(
    conditions... _conditions)
    : m_conditions(_conditions...) {}

template <std::integral auto num_of_supers, t_condition... conditions>
DryContract<num_of_supers, conditions...>::DryContract(
    Bases<num_of_supers> &&base, conditions... _conditions)
    : m_conditions(_conditions...), supers{std::move(base.b)} {}

template <std::integral auto num_of_supers, t_condition... conditions>
DryContract<num_of_supers, conditions...>::~DryContract() noexcept(false) {}

template <std::integral auto num_of_supers, t_condition... conditions>
void DryContract<num_of_supers, conditions...>::check_conditions(
    cond_type filt, std::experimental::source_location const &location) const {
  // if we are already in an exception, we don't need to add another error on
  // top of it.
  if (std::uncaught_exceptions())
    return;

  if constexpr (isGt(num_of_supers, 0))
    for (auto const &super : *supers)
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