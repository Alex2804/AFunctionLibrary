#ifndef AFUNCTIONLIBRARY_TOKEN_H
#define AFUNCTIONLIBRARY_TOKEN_H

#include "implementation/exports.h"

#include <algorithm>
#include <vector>
#include <memory>

#include "utility.h"
#include "tokenenums.h"

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
    AFUNCTIONLIBRARY_EXPORT std::ostream& operator<<(std::ostream& os, const Token<T>& token);


    template<typename T>
    class AFUNCTIONLIBRARY_EXPORT TokenAliases
    {
    public:
        TokenAliases() = default;
        explicit TokenAliases(TokenAliasType type);
        explicit TokenAliases(std::vector<T> aliases);
        TokenAliases(TokenAliasType type, std::vector<T> aliases);
        TokenAliases(const TokenAliases<T>& other) = default;
        TokenAliases(TokenAliases<T>&& other) noexcept = default;
        ~TokenAliases() = default;

        TokenAliases<T>& operator=(const TokenAliases<T>& other) = default;
        TokenAliases<T>& operator=(TokenAliases<T>&& other) noexcept = default;

        bool operator==(const TokenAliases& other) const;
        bool operator!=(const TokenAliases& other) const;

        TokenAliases<T>& append(const TokenAliases<T>& tokenAliases);
        TokenAliases<T>& append(std::vector<T> aliasVector);
        TokenAliases<T>& append(T alias);

        TokenAliasType type = TokenAliasType::String;
        std::vector<T> aliases;
    };


    template<typename T>
    class AFUNCTIONLIBRARY_NO_EXPORT TokenGroup
    {
    public:
        TokenGroup() = default;
        explicit TokenGroup(std::shared_ptr<const Token<T>> token);
        explicit TokenGroup(std::vector<size_t> groupID);
        TokenGroup(std::shared_ptr<const Token<T>> token, std::vector<size_t> groupID);
        TokenGroup(const TokenGroup<T>& other) = default;
        TokenGroup(TokenGroup<T>&& other) noexcept = default;
        ~TokenGroup() = default;

        TokenGroup<T>& operator=(const TokenGroup<T>& other) = default;
        TokenGroup<T>& operator=(TokenGroup<T>&& other) noexcept;

        bool operator==(const TokenGroup<T>& other) const;
        bool operator!=(const TokenGroup<T>& other) const;

        bool isToken() const;

        const T& getValue() const;
        T& getValue();
        TokenType getType() const;
        size_t getPrecedence() const;
        size_t getParameterCount() const;
        TokenAssociativity getAssociativity() const;

        std::shared_ptr<const Token<T>> token;
        std::vector<size_t> groupID;
    };

    template<typename T>
    AFUNCTIONLIBRARY_EXPORT std::ostream& operator<<(std::ostream& os, const TokenGroup<T>& tokenGroup);
}

#include "implementation/token.tpp"

#endif //AFUNCTIONLIBRARY_TOKEN_H