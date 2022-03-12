#include "../utility.hpp"
// import Wet
#include "DryContract.hpp"

namespace Contract_ns::Throwing {
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

template <std::integral auto num_of_supers, t_condition... conditions>
template <t_condition... added_conditions>
DryContract<num_of_supers, conditions..., added_conditions...>
DryContract<num_of_supers, conditions...>::MakeDry(
    added_conditions... _conditions) {
  return DryContract<num_of_supers, conditions..., added_conditions...>(
      *this, _conditions...);
}

template <std::integral auto num_of_supers, t_condition... conditions>
template <std::integral auto num_of_added_supers,
          t_condition... added_conditions>
DryContract<sum(num_of_supers, num_of_added_supers), conditions...,
            added_conditions...>
DryContract<num_of_supers, conditions...>::MakeDry(
    Bases<num_of_added_supers> &&Bases, added_conditions... _conditions) {
  return DryContract<sum(num_of_supers, num_of_added_supers), conditions...,
                     added_conditions...>(*this, Bases, _conditions...);
}
} // namespace Contract_ns::Throwing
