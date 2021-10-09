#ifndef PRE_POST_INVAR__HPP
#define PRE_POST_INVAR__HPP

#include "ThrowingCondition.hpp"

#include "../utility.hpp"

namespace Contract_ns::Throwing {

template <exception E>
constexpr auto cond_gen_default = [](cond_type type) {
  return [type](argumentless_function auto check_condition,
                std::string_view description) {
    return Condition<E, decltype(check_condition), decltype(&defExcGen)>(
        check_condition, description, type, &defExcGen);
  };
};

template <exception E>
constexpr auto cond_gen = [](cond_type type) {
  return [type](argumentless_function auto check_condition,
                std::string_view description, descGen auto DG) {
    return Condition<E, decltype(check_condition), decltype(DG)>(
        check_condition, description, type, DG);
  };
};

template <exception E>
constexpr overload pre{cond_gen_default<E>(precondition | invariant),
                       cond_gen<E>(precondition | invariant)};

template <exception E>
constexpr overload post{cond_gen_default<E>(postcondition | invariant),
                        cond_gen<E>(postcondition | invariant)};

template <exception E>
constexpr overload invar{cond_gen_default<E>(invariant),
                         cond_gen<E>(invariant)};

} // namespace Contract_ns::Throwing

#endif // PRE_POST_INVAR__HPP