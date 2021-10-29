#ifndef THROWING_CONDITION__HPP
#define THROWING_CONDITION__HPP

#include "../CommonCondition.hpp"
#include "../utility.hpp"

#include <optional>
#include <stdexcept>
#include <string_view>
#include <type_traits>
#include <utility>

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
  DG const generateExceptionDescription;

  bool is_violated(auto filt) const {
    return filter(filt) && !check_condition();
  };

  bool filter(auto filt) const noexcept { return filt & m_type; };
};

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

template <typename T>
concept t_condition = is_same_template<
    T, Condition<std::exception, decltype(empty), decltype(defExcGen)>>;

std::string defExcGen(std::experimental::source_location const &loc,
                      std::string_view description) {
  return fmt::format("{}: {}.\n{}: {}.\n{}: {}.",
                     "condition not met at the function", loc.function_name(),
                     "the error is", description, "line: ", loc.line());
}

} // namespace Contract_ns::Throwing

#endif // THROWING_CONDITION__HPP