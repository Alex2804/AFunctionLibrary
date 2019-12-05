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
            TokenPtrBundle(std::shared_ptr<const Token<T>> token, std::vector<TokenAliases < T>> aliases);
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

        template<typename T>
        class TokenGroup
        {
            TokenGroup() = default;
            TokenGroup(const TokenGroup<T>& other) = default;
            TokenGroup(TokenGroup<T>&& other) noexcept = default;

            TokenGroup<T>& operator=(const TokenGroup<T>& other) = default;
            TokenGroup<T>& operator=(TokenGroup<T>&& other) noexcept = default;

            const T& getValue() const;
            T& getValue();
            TokenType getType() const;
            size_t getPrecedence() const;
            size_t getParameterCount() const;
            TokenAssociativity getAssociativity() const;

            std::shared_ptr<const Token<T>> token;
            std::vector<TokenGroup<T>> tokens;
        };
    }
}

#include "implementation/tokenextras.tpp"

#endif //AFUNCTIONLIBRARY_TOKENEXTRAS_H
