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
        struct TokenBundle
        {
            Token<T> token;
            std::vector<TokenAliases<T>> aliases;
        };

        template<typename T>
        class TokenWrapper
        {
        public:
            TokenWrapper(std::shared_ptr<const Token<T>> token, std::vector<TokenAliases<T>> aliases);
            explicit TokenWrapper(TokenBundle<T> bundle);

            bool operator==(const TokenWrapper<T>& other) const;
            bool operator!=(const TokenWrapper<T>& other) const;

            std::shared_ptr<const Token<T>> token;
            std::vector<TokenAliases<T>> aliases;
        };
    }
}

#include "implementation/tokenextras.tpp"

#endif //AFUNCTIONLIBRARY_TOKENEXTRAS_H
