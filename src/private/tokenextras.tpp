#ifndef AFUNCTIONLIBRARY_TOKENEXTRAS_TPP
#define AFUNCTIONLIBRARY_TOKENEXTRAS_TPP


#include "tokenextras.h"

template<typename T>
afl::detail::TokenWrapper<T>::TokenWrapper(std::shared_ptr<const Token <T>> token, std::vector<TokenAliases> aliases)
    : token(std::move(token)), aliases(std::move(aliases))
{}

template<typename T>
afl::detail::TokenWrapper<T>::TokenWrapper(afl::detail::TokenBundle<T> bundle)
    : token(std::make_shared<const Token<T>>(std::move(bundle.token)))
    , aliases(std::move(bundle.aliases))
{

}

#endif //AFUNCTIONLIBRARY_TOKENEXTRAS_TPP