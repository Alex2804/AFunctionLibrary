#ifndef AFUNCTIONLIBRARY_TOKEN_TPP
#define AFUNCTIONLIBRARY_TOKEN_TPP

#include <AFunctionLibrary/token.h>

template<typename T>
afl::Token<T>::Token()
    : type(TokenType::Constant), precedence(0), parameterCount(0), associativity(TokenAssociativity::None)
{}
template<typename T>
afl::Token<T>::Token(T value, afl::TokenType type, int precedence, int parameterCount,
                     afl::TokenAssociativity associativity)
    : value(std::move(value)), type(type), precedence(precedence)
    , parameterCount(parameterCount), associativity(associativity)
{}

template<typename T>
bool afl::Token<T>::operator==(const afl::Token<T> &other) const
{
    return value == other.value && type == other.type && precedence == other.precedence
        && parameterCount == other.parameterCount && associativity == other.associativity;
}
template<typename T>
bool afl::Token<T>::operator!=(const afl::Token<T> &other) const
{
    return !operator==(other);
}

template<typename T>
bool afl::Token<T>::requirePrecedence()
{
    return type == TokenType::Operator;
}
template<typename T>
bool afl::Token<T>::requireAssociativity()
{
    return type == TokenType::Operator;
}
template<typename T>
bool afl::Token<T>::requireParameterCount()
{
    return type == TokenType::Function;
}

template<typename T>
bool afl::TokenAliases<T>::operator==(const TokenAliases &other) const
{
    return type == other.type && aliases == other.aliases;
}
template<typename T>
bool afl::TokenAliases<T>::operator!=(const TokenAliases &other) const
{
    return !operator==(other);
}

#endif //AFUNCTIONLIBRARY_TOKEN_TPP
