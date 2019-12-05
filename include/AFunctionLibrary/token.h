#ifndef AFUNCTIONLIBRARY_TOKEN_H
#define AFUNCTIONLIBRARY_TOKEN_H

#include "AFunctionLibrary/afunctionlibrary_export.h"

#include <set>

#include "AFunctionLibrary/implementation/cstringtoken.h"

namespace afl
{
    template<typename T>
    class AFUNCTIONLIBRARY_EXPORT Token
    {
    public:
        Token();
        Token(T value, TokenType type, int precedence, int parameterCount, TokenAssociativity associativity);
        Token(const Token<T>& other) = default;
        Token(Token<T>&& other) noexcept = default;
        virtual ~Token() = default;

        Token<T>& operator=(const Token<T>& other) = default;
        Token<T>& operator=(Token<T>&& other) noexcept = default;

        bool operator==(const Token<T>& other) const;
        bool operator!=(const Token<T>& other) const;

        bool requirePrecedence();
        bool requireAssociativity();
        bool requireParameterCount();

        const T& getValue() const;
        T& getValue();
        TokenType getType() const;
        size_t getPrecedence() const;
        size_t getParameterCount() const;
        TokenAssociativity getAssociativity() const;

        T value;
        TokenType type;
        size_t precedence;
        size_t parameterCount;
        TokenAssociativity associativity;
    };

    template<typename T>
    struct AFUNCTIONLIBRARY_EXPORT TokenAliases
    {
        bool operator==(const TokenAliases& other) const;
        bool operator!=(const TokenAliases& other) const;

        TokenAliasType type;
        std::set<T> aliases;
    };
}

#include "implementation/token.tpp"

#endif //AFUNCTIONLIBRARY_TOKEN_H
