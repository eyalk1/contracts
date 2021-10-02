#ifndef WET_CONTRACT__HPP
#define WET_CONTRACT__HPP

#include "DryContract.hpp"
#include "ThrowingCondition.hpp"

namespace Contract_ns::Throwing {

#define TCONTRACT(...)                                                         \
  Contract(std::experimental::source_location::current(), __VA_ARGS__)


template <bool has_super_contract, t_condition... conditions>
struct Contract : public DryContract<has_super_contract, conditions...> {
  using Base = DryContract<has_super_contract, conditions...>;

  Contract(std::experimental::source_location const &loc,
           super_init_list super, conditions... conds);

  Contract(std::experimental::source_location const &loc, conditions... conds);

  ~Contract() noexcept(false);
  std::experimental::source_location const &location;
};

/*************Class Template Deduction Guides*************/

template <t_condition... conditions>
Contract(std::experimental::source_location const &, conditions...)
    -> Contract<false, conditions...>;

template <t_condition... conditions>
Contract(std::experimental::source_location const &, super_init_list,
         conditions...) -> Contract<true, conditions...>;

/*************IMPLEMENTATION*************/
template <bool has_super_contract, t_condition... conditions>
Contract<has_super_contract, conditions...>::Contract(
    std::experimental::source_location const &loc, super_init_list super,
    conditions... conds)
    : Base(super, conds...), location(loc) {
  Base::check_conditions(precondition | invariant, location);
}

template <bool has_super_contract, t_condition... conditions>
Contract<has_super_contract, conditions...>::Contract(
    std::experimental::source_location const &loc, conditions... conds)
    : Base(conds...), location(loc) {
  Base::check_conditions(precondition | invariant, location);
}

template <bool has_super_contract, t_condition... conditions>
Contract<has_super_contract, conditions...>::~Contract() noexcept(false) {
  Base::check_conditions(postcondition | invariant, location);
}

} // namespace Contract_ns::Throwing
#endif // WET_CONTRACT__HPP