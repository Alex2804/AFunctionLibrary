#ifndef AFUNCTIONLIBRARY_TOKENEXTRAS_H
#define AFUNCTIONLIBRARY_TOKENEXTRAS_H

#include <vector>
#include <string>
#include <memory>

#include "AFunctionLibrary/token.h"

namespace afl
{
    namespace detail
    {
        template<typename T>
        struct AFUNCTIONLIBRARY_NO_EXPORT TokenInstanceBundle
        {
            Token<T> token;
            std::vector<TokenAliases<T>> aliases;
        };

        template<typename T>
        class AFUNCTIONLIBRARY_NO_EXPORT TokenPtrBundle
        {
        public:
            TokenPtrBundle(std::shared_ptr<const Token<T>> token, std::vector<TokenAliases<T>> aliases);
            explicit TokenPtrBundle(TokenInstanceBundle<T> bundle);

            TokenPtrBundle(const TokenPtrBundle<T>& other) = default;
            TokenPtrBundle(TokenPtrBundle<T>&& other) noexcept = default;

            TokenPtrBundle<T>& operator=(const TokenPtrBundle<T>& other) = default;
            TokenPtrBundle<T>& operator=(TokenPtrBundle<T>&& other) noexcept = default;

            bool operator==(const TokenPtrBundle<T>& other) const;
            bool operator!=(const TokenPtrBundle<T>& other) const;

            std::shared_ptr<const Token<T>> token;
            std::vector<TokenAliases<T>> aliases;
        };

        AFUNCTIONLIBRARY_NO_EXPORT std::string tokensToString(const std::vector<std::shared_ptr<const Token<std::string>>>& tokens);

        AFUNCTIONLIBRARY_NO_EXPORT std::vector<TokenGroup<std::string>> toGroupVector(const std::vector<std::shared_ptr<const Token<std::string>>>& tokens);
    }
}

#include "implementation/tokenextras.tpp"

#endif //AFUNCTIONLIBRARY_TOKENEXTRAS_H
