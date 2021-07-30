#ifndef CONDITION__HPP
#define CONDITION__HPP

#include <string_view>
#include <optional>
#include <utility>

enum cond_type{
    PRE = 1,
    INVARIANT =2,
    POST = 4
};

using cond_type_t = std::underlying_type_t<cond_type>;

template<typename F>
struct condition{
    cond_type m_type;

    F pred;

    std::string_view description;

    condition(cond_type _type, F _pred, std::string_view _descreption = "");

    template<typename new_f>
    auto operator=(std::pair<new_f, char const *> _pred);

    template<typename new_f>
    auto operator=(condition<new_f> rhs);
};

template<typename F>
condition(cond_type, F) -> condition<F>;

static condition pre(cond_type::PRE, []{});
static condition post(cond_type::POST, []{});
static condition invar(cond_type::INVARIANT, []{});

static auto precondition = cond_type::PRE;
static auto invariant = cond_type::INVARIANT;
static auto postcondition = cond_type::POST;


/*****************IMPLEMENTATION*****************/

template<typename F>
condition<F>::condition(cond_type _type, F _pred, std::string_view _descreption)
    :m_type(_type),pred(_pred), description(_descreption)
{};

template<typename F>
template<typename new_f>
auto condition<F>::operator=(std::pair<new_f, char const *> _pred){
    return condition<new_f>(this->m_type, _pred.first, _pred.second);
}


template<typename F>
template<typename new_f>
auto condition<F>::operator=(condition<new_f> rhs){
    rhs.m_type = this->m_type;
    return std::move(rhs);
}

#endif //CONDITION__HPP