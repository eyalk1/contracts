#ifndef MANUAL_CONTRACT__HPP
#define MANUAL_CONTRACT__HPP
#include "condition.hpp"

#include <boost/hana.hpp>
#include <experimental/source_location>
#include <iostream>



template<typename Error_t, typename ErrorHandler,typename... conditions>
struct ManualContract{
    ManualContract(ErrorHandler _f, conditions... _conditions);

    bool calculateError(cond_type_t to_check, std::experimental::source_location loc);

    bool check(cond_type_t check, std::experimental::source_location loc = std::experimental::source_location::current());

    Error_t error;
    ErrorHandler f;

    boost::hana::tuple<conditions...> const m_conditions;
};

/*****************IMPLEMENTATION*****************/

template<typename Error_t, typename ErrorHandler,typename... conditions>
ManualContract<Error_t, ErrorHandler, conditions...>::ManualContract(ErrorHandler _f, conditions... _conditions)
    :m_conditions(_conditions...), f(_f) {
}

template<typename Error_t, typename ErrorHandler,typename... conditions>
bool ManualContract<Error_t, ErrorHandler, conditions...>::check(cond_type_t check, std::experimental::source_location loc){
    return calculateError(check, std::move(loc));
}

template<typename Error_t, typename ErrorHandler,typename... conditions>
bool ManualContract<Error_t, ErrorHandler, conditions...>::calculateError(cond_type_t to_check, std::experimental::source_location loc){
    bool erred_yet{false};
    std::string_view description;
    int EC{0};

    boost::hana::for_each(m_conditions, [to_check, &erred_yet, &description, &EC](auto &condition){
        if(auto _ec = condition.pred();
            !erred_yet &&
            (condition.m_type & to_check) &&
            _ec){
            erred_yet = true;
            description = condition.description;
            EC = _ec;
        }
    });

    if(description.empty())
        return true;
    error = f(loc, description, EC);
    return false;
}

#endif //MANUAL_CONTRACT__HPP