#ifndef AFUNCTIONLIBRARY_TOKENEXTRAS_TPP
#define AFUNCTIONLIBRARY_TOKENEXTRAS_TPP


#include "../tokenextras.h"

template<typename T>
afl::detail::TokenWrapper<T>::TokenWrapper(std::shared_ptr<const Token<T>> token, std::vector<TokenAliases<T>> aliases)
    : token(std::move(token)), aliases(std::move(aliases))
{}

template<typename T>
afl::detail::TokenWrapper<T>::TokenWrapper(afl::detail::TokenBundle<T> bundle)
    : token(std::make_shared<const Token<T>>(std::move(bundle.token)))
    , aliases(std::move(bundle.aliases))
{

}

template<typename T>
bool afl::detail::TokenWrapper<T>::operator==(const afl::detail::TokenWrapper<T> &other) const
{
    return *token == *other.token && aliases == other.aliases;
}
template<typename T>
bool afl::detail::TokenWrapper<T>::operator!=(const afl::detail::TokenWrapper<T> &other) const
{
    return !operator==(other);
}

#endif //AFUNCTIONLIBRARY_TOKENEXTRAS_TPP