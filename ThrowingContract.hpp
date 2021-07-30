#include "condition.hpp"

#include <boost/hana.hpp>
#include <stdexcept>
#include <sstream>
#include <experimental/source_location>

#define TCONTRACT(...) ThrowingContract(std::experimental::source_location::current(),__VA_ARGS__)

std::runtime_error GenerateException(std::string_view location, std::string_view failed){
    std::stringstream ss;
    ss <<  "condition not met at entrance to the function " << location << " .the error is: " << failed;
    return std::runtime_error(ss.str());
}



template<typename... conditions>
struct ThrowingContract
{
    ThrowingContract(std::experimental::source_location _location, conditions... _conditions);
    ~ThrowingContract();

    std::experimental::source_location location;
    boost::hana::tuple<conditions...> const m_conditions;
};


/*****************IMPLEMENTATION*****************/


template<typename... conditions>
ThrowingContract<conditions...>::ThrowingContract(std::experimental::source_location _location, conditions... _conditions):
    m_conditions(_conditions...), location(_location) {
    boost::hana::for_each(m_conditions, [this](auto const & condition){
        if(condition.m_type != cond_type::POST && !(condition.pred()))
            throw GenerateException(this->location.function_name(), condition.description);
    });
}

template<typename... conditions>
ThrowingContract<conditions...>::~ThrowingContract(){
    boost::hana::for_each(m_conditions, [this](auto const & condition){
        if(condition.m_type != cond_type::PRE && !(condition.pred()))
            throw GenerateException(this->location.function_name(), condition.description);
    });
}