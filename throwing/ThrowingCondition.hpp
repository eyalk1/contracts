#ifndef THROWING_CONDITION__HPP
#define THROWING_CONDITION__HPP

#include "../CommonCondition.hpp"
#include "../utility.hpp"

#include <stdexcept>
#include <string_view>
#include <type_traits>
#include <utility>

namespace Contract_ns::Throwing {

std::string
generateExceptionDescription(std::experimental::source_location const &loc,
                             std::string_view description);

template <exception Exc, argumentless_function F> struct Condition {
  constexpr Condition(F _pred, std::string_view _description,
                      cond_type _type = precondition);
  Exc getException(std::experimental::source_location const &loc) const;
  F const pred;
  std::string_view const description;
  cond_type m_type;

  bool does_violate(auto filt) const { return filter(filt) && !pred();};

  bool filter(auto filt) const { return filt & m_type; };

  /**
   * @brief create and return a condition, override it's type with this's type.
   *
   * @tparam new_f predicate of new condition
   * @param _pred predicate of new condition
   * @return condition<new_f> return the new constructed predicate
   */
  template <exception new_Exc, argumentless_function new_f>
  Condition<new_Exc, new_f> operator=(Condition<new_Exc, new_f> cond) const;
};

template <exception Exc, argumentless_function F>
constexpr Condition<Exc, F>::Condition(F _pred, std::string_view _description,
                                       cond_type _type)
    : pred(_pred), description(_description), m_type(_type) {}

template <exception Exc, argumentless_function F>
Exc Condition<Exc, F>::getException(
    std::experimental::source_location const &loc) const {
  return Exc(generateExceptionDescription(loc, description));
};

template <exception Exc, argumentless_function F>
template <exception new_Exc, argumentless_function new_f>
Condition<new_Exc, new_f>
Condition<Exc, F>::operator=(Condition<new_Exc, new_f> cond) const {
  return Condition<new_Exc, new_f>(cond.pred, cond.description, cond.m_type);
}

/*****************UTILITIES*****************/

template <exception E>
auto condition_generator = [](cond_type type) {
  return [type](auto pred, std::string_view description) {
    return Condition<E, decltype(pred)>(pred, description, type);
  };
};

template <exception E> auto pre = condition_generator<E>(precondition);

template <exception E> auto post = condition_generator<E>(postcondition);

template <exception E> auto invar = condition_generator<E>(invariant);

template <typename T>
concept t_condition = is_same_template<T, Condition<std::exception, decltype(empty)>>;

std::string
generateExceptionDescription(std::experimental::source_location const &loc,
                             std::string_view description) {
  return fmt::format("{}: {}.\n{}: {}.\n{}: {}",
                     "condition not met at the function", loc.function_name(),
                     "the error is", description, "line: ", loc.line());
}

} // namespace Contract_ns::Throwing

#endif // THROWING_CONDITION__HPP