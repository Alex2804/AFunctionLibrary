#ifndef AFUNCTIONLIBRARY_TOKENEXTRAS_H
#define AFUNCTIONLIBRARY_TOKENEXTRAS_H

#include <vector>
#include <string>
#include <memory>
#include <set>

#include "AFunctionLibrary/token.h"

namespace afl
{
    namespace detail
    {
        enum class TokenAliasType
        {
            String,
            Regex
        };

        struct TokenAliases
        {
            TokenAliasType type;
            std::set<std::string> aliases;
        };

        template<typename T>
        struct TokenBundle
        {
            Token<T> token;
            std::vector<TokenAliases> aliases;
        };

        template<typename T>
        class TokenWrapper
        {
        public:
            TokenWrapper(std::shared_ptr<const Token<T>> token, std::vector<TokenAliases> aliases);
            explicit TokenWrapper(TokenBundle<T> bundle);

            std::shared_ptr<const Token<T>> token;
            std::vector<TokenAliases> aliases;
        };
    }
}

#include "tokenextras.tpp"

#endif //AFUNCTIONLIBRARY_TOKENEXTRAS_H
