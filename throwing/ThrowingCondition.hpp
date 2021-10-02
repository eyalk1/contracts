#ifndef THROWING_CONDITION__HPP
#define THROWING_CONDITION__HPP

#include "../CommonCondition.hpp"
#include "../utility.hpp"

#include <optional>
#include <stdexcept>
#include <string_view>
#include <type_traits>
#include <utility>

template <typename F>
concept descGen = requires(F f) {
  std::is_invocable_v<F, std::experimental::source_location const &,
                      std::string_view>;
  std::is_same_v<std::string, decltype(f)>;
  //  std::result_of_t<F, std::experimental::source_location const &,
  //                   std::string_view>>;
};

namespace Contract_ns::Throwing {


std::string defExcGen(std::experimental::source_location const &loc,
                      std::string_view description);

template <exception Exc, argumentless_function F, descGen DG> struct Condition {
  constexpr Condition(F _pred, std::string_view _description, cond_type _type,
                      DG dg);
  constexpr Condition(F _pred, std::string_view _description, cond_type _type);
  Exc getException(std::experimental::source_location const &loc) const;
  F const check_condition;
  std::string_view const description;
  cond_type const m_type;
  DG generateExceptionDescription;

  bool is_violated(auto filt) const {
    return filter(filt) && !check_condition();
  };

  bool filter(auto filt) const { return filt & m_type; };
};

// template <exception Exc, argumentless_function F, descGen DG>
// Condition(F, std::string_view, cond_type, DG)
//     -> Condition<Exc, F, DG>;

// template <exception Exc, argumentless_function F>
// Condition(F, std::string_view, cond_type)
//     -> Condition<Exc, F, decltype(defExcGen)>;

template <exception Exc, argumentless_function F, descGen DG>
constexpr Condition<Exc, F, DG>::Condition(F _pred,
                                           std::string_view _description,
                                           cond_type _type, DG dg)
    : check_condition(_pred), description(_description), m_type(_type),
      generateExceptionDescription(dg) {}

template <exception Exc, argumentless_function F, descGen DG>
constexpr Condition<Exc, F, DG>::Condition(F _pred,
                                           std::string_view _description,
                                           cond_type _type)
    : check_condition(_pred), description(_description), m_type(_type),
      generateExceptionDescription(defExcGen) {}

template <exception Exc, argumentless_function F, descGen DG>
Exc Condition<Exc, F, DG>::getException(
    std::experimental::source_location const &loc) const {
  return Exc(generateExceptionDescription(loc, description));
};

/*****************UTILITIES*****************/

template <exception E>
auto condition_generator = [](cond_type type) {
  return [type](argumentless_function auto check_condition,
                std::string_view description) {
    return Condition<E, decltype(check_condition), decltype(defExcGen)>(
        check_condition, description, type, defExcGen);
  };
};

// template <exception E>
// auto condition_generator_2 = [](cond_type type) {
//   return [type](argumentless_function auto check_condition,
//                 std::string_view description, typename auto DG) {
//     return Condition<E, decltype(check_condition), decltype(DG)>(
//         check_condition, description, type, DG);
//   };
// };

// template <exception E> auto pre = condition_generator<E>(precondition);

template <exception E> auto post = condition_generator<E>(postcondition);

template <exception E> auto invar = condition_generator<E>(invariant);

template <typename T>
concept t_condition = is_same_template<
    T, Condition<std::exception, decltype(empty), decltype(defExcGen)>>;

std::string defExcGen(std::experimental::source_location const &loc,
                      std::string_view description) {
  return fmt::format("{}: {}.\n{}: {}.\n{}: {}",
                     "condition not met at the function", loc.function_name(),
                     "the error is", description, "line: ", loc.line());
}

} // namespace Contract_ns::Throwing

#endif // THROWING_CONDITION__HPP