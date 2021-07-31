#ifndef COMMON_CONDITION__HPP
#define COMMON_CONDITION__HPP

enum cond_type { PRE = 1, INVARIANT = 2, POST = 4 };

/**
 * makes it a bit less tedious to write conditions.
 */
static constexpr auto precondition = cond_type::PRE;
static constexpr auto invariant = cond_type::INVARIANT;
static constexpr auto postcondition = cond_type::POST;


#endif //COMMON_CONDITION__HPP