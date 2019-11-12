#ifndef AFUNCTIONLIBRARY_TOKEN_H
#define AFUNCTIONLIBRARY_TOKEN_H

#include "AFunctionLibrary/afunctionlibrary_export.h"

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

        bool requirePrecedence();
        bool requireAssociativity();
        bool requireParameterCount();

        T value;
        TokenType type;
        size_t precedence;
        size_t parameterCount;
        TokenAssociativity associativity;
    };
}

#include "implementation/token.tpp"

#endif //AFUNCTIONLIBRARY_TOKEN_H
