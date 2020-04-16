#ifndef AFUNCTIONLIBRARYSDK_TOKENACCESSINTERFACE_HPP
#define AFUNCTIONLIBRARYSDK_TOKENACCESSINTERFACE_HPP

#include <cstddef>

#include "../tokenenums.h"

namespace afl
{
    template<typename T>
    inline TokenType getType(const T& token);
    template<typename T>
    inline size_t getPrecedence(const T& token);
    template<typename T>
    inline size_t getParameterCount(const T& token);
    template<typename T>
    inline TokenAssociativity getAssociativity(const T& token);

    namespace detail
    {
        template<typename... Ts> struct make_void { typedef void type;};
        template<typename... Ts> using void_t = typename make_void<Ts...>::type;

        template< bool Condition, typename T = void >
        using enable_if_t = typename std::enable_if<Condition, T>::type;

        template<template<class...> class Z, class, class...>
        struct can_apply : std::false_type {};
        template<template<class...> class Z, class...Ts>
        struct can_apply<Z, void_t<Z<Ts...>>, Ts...> : std::true_type {};

        template<class, class = void>
        struct is_dereferenceable : std::false_type {};
        template<class T>
        struct is_dereferenceable<T, void_t<decltype(*std::declval<T>())>> : std::true_type {};

        template<class, class = void>
        struct is_arrow_dereferenceable : std::false_type {};
        template<class T>
        struct is_arrow_dereferenceable<T*, void> : std::true_type {};
        template<class T>
        struct is_arrow_dereferenceable<T, void_t<decltype(std::declval<T>().operator->())>> : std::true_type {};

        template<typename T>
        inline TokenType getType(const T& token, std::true_type);
        template<typename T>
        inline TokenType getType(const T& token, std::false_type);

        template<typename T>
        inline size_t getPrecedence(const T& token, std::true_type);
        template<typename T>
        inline size_t getPrecedence(const T& token, std::false_type);

        template<typename T>
        inline size_t getParameterCount(const T& token, std::true_type);
        template<typename T>
        inline size_t getParameterCount(const T& token, std::false_type);

        template<typename T>
        inline TokenAssociativity getAssociativity(const T& token, std::true_type);
        template<typename T>
        inline TokenAssociativity getAssociativity(const T& token, std::false_type);
    }
}


template<typename T>
inline afl::TokenType afl::detail::getType(const T& token, std::true_type) {
    return token->getType();
}
template<typename T>
inline afl::TokenType afl::detail::getType(const T& token, std::false_type) {
    return token.getType();
}
template<typename T>
inline afl::TokenType afl::getType(const T& token) {
    return getType(token, detail::is_arrow_dereferenceable<T>{});
}

template<typename T>
inline size_t afl::detail::getPrecedence(const T& token, std::true_type) {
    return token->getPrecedence();
}
template<typename T>
inline size_t afl::detail::getPrecedence(const T& token, std::false_type) {
    return token.getPrecedence();
}
template<typename T>
inline size_t afl::getPrecedence(const T& token) {
    return getPrecedence(token, detail::is_arrow_dereferenceable<T>{});
}

template<typename T>
inline size_t afl::detail::getParameterCount(const T& token, std::true_type) {
    return token->getParameterCount();
}
template<typename T>
inline size_t afl::detail::getParameterCount(const T& token, std::false_type) {
    return token.getParameterCount();
}
template<typename T>
inline size_t afl::getParameterCount(const T& token) {
    return getParameterCount(token, detail::is_arrow_dereferenceable<T>{});
}

template<typename T>
inline afl::TokenAssociativity afl::detail::getAssociativity(const T& token, std::true_type) {
    return token->getAssociativity();
}
template<typename T>
inline afl::TokenAssociativity afl::detail::getAssociativity(const T& token, std::false_type) {
    return token.getAssociativity();
}
template<typename T>
inline afl::TokenAssociativity afl::getAssociativity(const T& token) {
    return getAssociativity(token, detail::is_arrow_dereferenceable<T>{});
}

#endif //AFUNCTIONLIBRARYSDK_TOKENACCESSINTERFACE_HPP
