#ifndef AFUNCTIONLIBRARY_TOKENEXTRAS_TPP
#define AFUNCTIONLIBRARY_TOKENEXTRAS_TPP

template<typename T>
afl::detail::TokenPtrBundle<T>::TokenPtrBundle(std::shared_ptr<const Token<T>> token, std::vector<TokenAliases<T>> aliases)
: token(std::move(token)), aliases(std::move(aliases))
{}
template<typename T>
afl::detail::TokenPtrBundle<T>::TokenPtrBundle(afl::detail::TokenInstanceBundle<T> bundle)
        : token(std::make_shared<const Token<T>>(std::move(bundle.token))), aliases(std::move(bundle.aliases))
{}

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

#endif //AFUNCTIONLIBRARY_TOKENEXTRAS_TPP