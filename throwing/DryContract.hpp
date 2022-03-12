#ifndef THROWING_CONTRACT__HPP
#define THROWING_CONTRACT__HPP

#include "Condition.hpp"
#include "IContract.hpp"

#include "Bases.hpp"

#include <boost/hana.hpp>
#include <experimental/source_location>
#include <fmt/core.h>
#include <functional>
#include <sstream>
#include <string>
#include <vector>

namespace hana = boost::hana;

namespace Contract_ns::Throwing {
template <std::integral auto num_of_supers>
using super_list_t =
    std::array<std::reference_wrapper<IContract const>, num_of_supers>;

template <std::integral auto num_of_supers = 0, t_condition... conditions>
struct Contract;

template <std::integral auto num_of_supers = 0, t_condition... conditions>
struct DryContract : public IContract {
  using super_list = super_list_t<num_of_supers>;
  /**************constructors**************/
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

  /**************make Dry/ make Wet**************/
  template <t_condition... additional_conditions>
  DryContract<num_of_supers, conditions..., additional_conditions...>
  MakeDry(additional_conditions... _conditions);

  template <std::integral auto additional_supers,
            t_condition... additional_conditions>
  DryContract<sum(num_of_supers ,additional_supers), conditions... ,additional_conditions...>
  MakeDry(Bases<additional_supers> &&Bases,
          additional_conditions... _conditions);

  template <std::integral auto additional_supers,
            t_condition... additional_conditions>
  Contract<additional_supers, additional_conditions...>
  MakeWet(std::experimental::source_location loc,
          Bases<additional_supers> &&super, additional_conditions &&...conds);

  template <std::integral auto additional_supers,
            t_condition... additional_conditions>
  Contract<additional_supers, additional_conditions...>
  MakeWet(std::experimental::source_location loc,
          additional_conditions &&...conds);

  /**
   * @brief Destroy the Throwing DryContract object. go over the post and
   * invariant conditions.
   */
  virtual ~DryContract() noexcept(false);

  DryContract(DryContract const &) = delete;
  DryContract(DryContract &&) = delete;
  auto operator=(DryContract const &) = delete;
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
  template <std::integral auto num_of_other_supers = 0,
            t_condition... other_conditions, t_condition... added_conditions>
  DryContract(DryContract<num_of_other_supers, other_conditions...> &other,
              added_conditions... _conditions)
      : m_conditions(hana::flatten(hana::make_tuple(
            other.m_conditions, hana::make_tuple(_conditions...)))),
        supers{other.supers} {};

  template <std::integral auto num_of_other_supers = 0,
            std::integral auto additional_supers = 0,
            t_condition... other_conditions,
            t_condition... added_conditions>
  DryContract(DryContract<num_of_other_supers, other_conditions...> &other,
              Bases<additional_supers> &&bases, added_conditions... _conditions)
      : m_conditions(hana::flatten(hana::make_tuple(
            other.m_conditions, hana::make_tuple(_conditions...)))),
        supers{other.supers, std::move(bases.b)} {};

  boost::hana::tuple<conditions...> const m_conditions;
  contain_if<isGt(num_of_supers, 0), super_list> supers;

  template <std::integral auto num, t_condition... conds>
  friend struct DryContract;
};

/****************DEDUCTION GUIDES****************/

template <base_t sl, t_condition... conditions>
DryContract(sl &&, conditions...) -> DryContract<sl::size, conditions...>;

template <t_condition... conditions>
DryContract(conditions...) -> DryContract<0, conditions...>;

template <typename T>
concept t_contract = is_same_template<T, DryContract<>>;

} // namespace Contract_ns::Throwing

#include "DryContract.tpp"

#endif // THROWING_CONTRACT__HPP