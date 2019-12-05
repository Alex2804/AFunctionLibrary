#ifndef AFUNCTIONLIBRARY_TOKENEXTRAS_TPP
#define AFUNCTIONLIBRARY_TOKENEXTRAS_TPP

#include "../tokenextras.h"

template<typename T>
afl::detail::TokenPtrBundle<T>::TokenPtrBundle(std::shared_ptr<const Token<T>> token, std::vector<TokenAliases < T>> aliases)
    : token(std::move(token)), aliases(std::move(aliases))
{}

template<typename T>
afl::detail::TokenPtrBundle<T>::TokenPtrBundle(afl::detail::TokenInstanceBundle<T> bundle)
    : token(std::make_shared<const Token<T>>(std::move(bundle.token)))
    , aliases(std::move(bundle.aliases))
{

}

template<typename T>
bool afl::detail::TokenPtrBundle<T>::operator==(const afl::detail::TokenPtrBundle<T> &other) const
{
    return *token == *other.token && aliases == other.aliases;
}
template<typename T>
bool afl::detail::TokenPtrBundle<T>::operator!=(const afl::detail::TokenPtrBundle<T> &other) const
{
    return !operator==(other);
}

template<typename T>
const T &afl::detail::TokenGroup<T>::getValue() const
{
    return token != nullptr ? token->value : T();
}
template<typename T>
T &afl::detail::TokenGroup<T>::getValue()
{
    return token != nullptr ? token->value : T();
}
template<typename T>
afl::TokenType afl::detail::TokenGroup<T>::getType() const
{
    return token != nullptr ? token->type : TokenType::Constant;
}
template<typename T>
size_t afl::detail::TokenGroup<T>::getPrecedence() const
{
    return token != nullptr ? token->precedence : 0;
}
template<typename T>
size_t afl::detail::TokenGroup<T>::getParameterCount() const
{
    return token != nullptr ? token->parameterCount : 0;
}
template<typename T>
afl::TokenAssociativity afl::detail::TokenGroup<T>::getAssociativity() const
{
    return token != nullptr ? token->associativity : TokenAssociativity::None;
}

#endif //AFUNCTIONLIBRARY_TOKENEXTRAS_TPP