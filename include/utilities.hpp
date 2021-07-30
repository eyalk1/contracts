#ifndef UTILITY__HPP
#define UTILITY__HPP


template <template <typename...> class, template<typename...> class>
struct is_same_template{
    static constexpr auto value = false;
};

template <template <typename...> class T>
struct is_same_template<T,T>{
    static constexpr auto value = true;
};

template<template <typename...> class T, template<typename...> class U>
static constexpr auto is_same_template_v = is_same_template<T,U>::value;

#endif //UTILITY__HPP