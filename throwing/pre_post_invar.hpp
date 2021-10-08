#ifndef PRE_POST_INVAR__HPP
#define PRE_POST_INVAR__HPP

#include "ThrowingCondition.hpp"

#include "../utility.hpp"

template <exception E>
auto condition_generator = [](cond_type type) {
  return [type](argumentless_function auto check_condition,
                std::string_view description) {
    return Condition<E, decltype(check_condition), decltype(&defExcGen)>(
        check_condition, description, type, &defExcGen);
  };
};

template <exception E>
auto condition_generator_2 = [](cond_type type) {
  return [type](argumentless_function auto check_condition,
                std::string_view description, typename auto DG) {
    return Condition<E, decltype(check_condition), decltype(DG)>(
        check_condition, description, type, DG);
  };
};

// template <exception E> auto pre = condition_generator<E>(precondition);
template <exception E>
auto pre1 = [](argumentless_function auto f, std::string_view sv,
              Contract_ns::cond_type type) {
  return Condition<std::logic_error, decltype(f), decltype(&defExcGen)>(f, sv,
                                                                       type, &defExcGen);
};

template <exception E>
auto pre2 = [](argumentless_function auto f, std::string_view sv,
              Contract_ns::cond_type type, descGen auto dg) {
  return Condition<std::logic_error, decltype(f), decltype(dg)>(f, sv, type,
                                                                dg);
};

template<exception E>
overload pre{pre1<E>, pre2<E>};



template <exception E> auto post = condition_generator<E>(postcondition);

template <exception E> auto invar = condition_generator<E>(invariant);


#endif //PRE_POST_INVAR__HPP