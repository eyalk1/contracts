#ifndef COMMON_CONDITION__HPP
#define COMMON_CONDITION__HPP

#include "utility.hpp"
#include <type_traits>

namespace Contract_ns {

enum cond_type { PRE = 1, INVARIANT = 2, POST = 4 };



using cond_type_t = std::underlying_type_t<cond_type>;

/**
 * makes it a bit less tedious to write conditions.
 */
static constexpr auto precondition = cond_type::PRE;
static constexpr auto invariant = cond_type::INVARIANT;
static constexpr auto postcondition = cond_type::POST;

} // namespace Contract_ns
template<>
constexpr bool EnableBitMaskOperators<Contract_ns::cond_type> = true;


#endif // COMMON_CONDITION__HPP