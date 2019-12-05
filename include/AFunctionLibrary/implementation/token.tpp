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
const T& afl::Token<T>::getValue() const
{
    return value;
}
template<typename T>
T& afl::Token<T>::getValue()
{
    return value;
}
template<typename T>
afl::TokenType afl::Token<T>::getType() const
{
    return type;
}

template<typename T>
size_t afl::Token<T>::getPrecedence() const
{
    return precedence;
}

template<typename T>
size_t afl::Token<T>::getParameterCount() const
{
    return parameterCount;
}
template<typename T>
afl::TokenAssociativity afl::Token<T>::getAssociativity() const
{
    return associativity;
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
