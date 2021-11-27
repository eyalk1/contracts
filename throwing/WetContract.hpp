#ifndef WET_CONTRACT__HPP
#define WET_CONTRACT__HPP

#include "DryContract.hpp"
#include "ThrowingCondition.hpp"

#include <concepts>

namespace Contract_ns::Throwing {

#define CONTRACT(...)                                                          \
  Contract(std::experimental::source_location::current(), __VA_ARGS__)

template <std::integral auto num_of_supers, t_condition... conditions>
struct Contract : public DryContract<num_of_supers, conditions...> {
  using Base = DryContract<num_of_supers, conditions...>;

  Contract(std::experimental::source_location loc, Bases<num_of_supers> &&super,
           conditions&&... conds);

  Contract(std::experimental::source_location loc, conditions&&... conds);

  Contract(Contract const&) = delete;
  Contract(Contract &&) = delete;
  auto operator=(Contract const&) = delete;
  auto operator=(Contract &&) = delete;

  virtual ~Contract() noexcept(false);
  std::experimental::source_location location;
};

/*************Class Template Deduction Guides*************/

template <t_condition... conditions>
Contract(std::experimental::source_location, conditions...)
    -> Contract<0, conditions...>;

template <std::integral auto num_of_supers, t_condition... conditions>
Contract(std::experimental::source_location, Bases<num_of_supers> &&,
         conditions...) -> Contract<num_of_supers, conditions...>;

/*************IMPLEMENTATION*************/

template <std::integral auto num_of_supers, t_condition... conditions>
Contract<num_of_supers, conditions...>::Contract(
    std::experimental::source_location loc, Bases<num_of_supers> && super,
    conditions&&... conds)
    : Base(std::move(super), std::move(conds)...), location(loc) {
  Base::check_conditions(precondition | invariant, location);
}

template <std::integral auto num_of_supers, t_condition... conditions>
Contract<num_of_supers, conditions...>::Contract(
    std::experimental::source_location loc, conditions&&... conds)
    : Base(std::move(conds)...), location(loc) {
  Base::check_conditions(precondition | invariant, location);
}

template <std::integral auto num_of_supers, t_condition... conditions>
Contract<num_of_supers, conditions...>::~Contract() noexcept(false) {
  Base::check_conditions(postcondition | invariant, location);
}

} // namespace Contract_ns::Throwing
#endif // WET_CONTRACT__HPP