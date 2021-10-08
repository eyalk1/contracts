#ifndef PRE_POST_INVAR__HPP
#define PRE_POST_INVAR__HPP

#include "ThrowingCondition.hpp"

#include "../utility.hpp"


namespace Contract_ns::Throwing {

template <exception E>
auto cond_gen_default = [](int type) {
  return [type](argumentless_function auto check_condition,
                std::string_view description) {
    return Condition<E, decltype(check_condition), decltype(&defExcGen)>(
        check_condition, description, type, &defExcGen);
  };
};

template <exception E>
auto cond_gen = [](int type) {
  return [type](argumentless_function auto check_condition,
                std::string_view description, descGen auto DG) {
    return Condition<E, decltype(check_condition), decltype(DG)>(
        check_condition, description, type, DG);
  };
};

template <exception E>
auto pre1 = cond_gen_default<E>(precondition | invariant);

template <exception E>
auto pre2 = cond_gen<E>(precondition | invariant);

template <exception E>
auto post1 = cond_gen_default<E>(invariant | postcondition);

template <exception E>
auto post2 = cond_gen<E>(invariant | postcondition);

template <exception E>
auto invar1 = cond_gen_default<E>(invariant);

template <exception E>
auto invar2 = cond_gen<E>(invariant);


template<exception E>
overload pre{pre1<E>, pre2<E>};

template<exception E>
overload post{post1<E>, pre2<E>};

template<exception E>
overload invar{invar1<E>, invar2<E>};

} // namespace Contract_ns::Throwing

#endif //PRE_POST_INVAR__HPP